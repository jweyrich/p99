#!/bin/sh -f
#!perl     ##  This may look like complete nonsense, but it really is -*- mode: perl; coding: utf-8 -*- ##
#          ##  Exec to perl if started from /bin/sh. Do nothing if already executed by perl.            ##
# line 5
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;
# Except of parts copied from previous work and as explicitly stated below,
# the authors and copyright holders for this work are as follows:
# all rights reserved,  2011 Jens Gustedt, INRIA, France
# all rights reserved,  2011 Matias E. Vara, INRIA, France
#
# This file is part of the P99 project. You received this file as as
# part of a confidential agreement and you may generally not
# redistribute it and/or modify it, unless under the terms as given in
# the file LICENSE.  It is distributed without any warranty; without
# even the implied warranty of merchantability or fitness for a
# particular purpose.
#
#
# Usage: parallel_coloreo.pl [file] [Parameters]
#
# Notes:
#
# - [file] is in .dot format, "label" sentences are used for naming
#   the vertex but they aren't necessary, comments and other kind of
#   information is ignored.
# - The vertex must be identify using numbers from "0" to "N".
#
use English;
use strict;
use Getopt::Long;
use Thread;

# colors[X] is an array which every entry is a list of vertex that they are coloring with X color.
# It is filled at run time.
my %colors;

# List of colors already used
my @colors_defined;

# vertex' names
my %nom;

# colored_vertex[X] holds the color of vertex X.
my @colored_vertex : shared;

# The hash holds the same (local) data, but just ... hashed.
my %colored_vertex;

# graph[X] is a list of vertices that are connected with X.
# It is filled when the application starts.
my @graph;

# assignment uses a simple counter
# WARNING that variable is used by the threads but it not protected!
my $last_color_used = 0;

# Thread List
my @t;

# parameter passing
my $ifile;
my $ofile;
my $number;
my $check;
my $ths = 2;
my $VxperTh;

# Maximun number of color used, at the moment this paramter is fixed.
# Using: < Delta + 1 as a limited.
# In matrix case 15 is the Max degree.
# In the worse case it uses $color_palette colors.
# TODO: transform this into a parameter
my $color_palette = 18;

# getting the parameters
GetOptions (
    'number'=> \$number,
    'check'=> \$check,
    'threads=i' => \$ths,
    'colors|c=i' => \$color_palette,
    'ifile|i=s' => \$ifile,
    'ofile|o=s' => \$ofile,
    );

if (!$ifile) {
    $ifile = $ARGV[0];
}

if (!$ifile) {
    print "Usage: $PROGRAM_NAME [Parameters] [file]\n";
    exit;
}

sub is_local($$$);
sub color_node($$);
sub uncolor_node($$);
sub has_color($);
sub get_colorID($);
sub get_colorID2($);
sub is_my_neighbor($$);
sub is_bad_color($$);
sub get_color($);
sub get_color2($);
sub communicate_coloring($$);
sub send_colors($);
sub receiv_colors($);
sub do_coloring($$$);
sub is_bad_color_main($$);
sub get_color_main($);
sub fix_conflicts();
sub check_conflicts();
sub make_output($);
sub make_graph();
sub create_vertex(@);
sub create_edge($$);



sub create_vertex(@) {
    foreach (@ARG) {
        if (!defined($graph[$_])) {
            $graph[$_] = [];
            my %ndata;
            $nom{$_}  = \%ndata;
        }
    }
}

sub create_edge($$) {
    create_vertex(@ARG);
    my ($id, $neigh) = @ARG;
    push @{$graph[$id]}, $neigh;
    push @{$graph[$neigh]}, $id;
}


# the information as found in the graph
my $graphhead = "";
my $graphlist = "";

#
# is_local:
#
# $ARG[0]: Counter start.
# $ARG[1]: Counter end.
# $ARG[2]: Vertex ID.
#
# Return true if the thread has the Vertex.
# TODO: This procedure must be re-written in a distributed environment.
#
sub is_local($$$) {
    if (($ARG[2] <= $ARG[1]) && ($ARG[0] <= $ARG[2])) {
        return 1;
    }
}

## color a node
sub color_node($$) {
    my ($color, $node) = @ARG;
    push @{ $colors{$color} }, $node;
    $colored_vertex{$node} = $color;
}

sub uncolor_node($$) {
    my ($color, $node) = @ARG;
    delete $colors{$color}->[$node];
    delete $colored_vertex{$node};
}

# has_color:
#
# $ARG[0]: vertex ID.
#
# return true if a vertex has been colored
#
sub has_color($) {
    defined($colored_vertex{$ARG[0]});
}

# get_colorID:
#
# $ARG[0]: vertex ID.
#
# return vertex's color
#
sub get_colorID($) {
    if (defined($colored_vertex{$ARG[0]})) {
        $colored_vertex{$ARG[0]};
    } else {
        -1;
    }
}
# similar than get_colorID, this one returns $j (position into the color queue) while the other return $i (color)
sub get_colorID2($)
{
    if (defined($colored_vertex{$ARG[0]})) {
        my $color = $colored_vertex{$ARG[0]};
        for (my $j = 0; $j <= $#{ $colors{$color} }; $j++) {
            if (defined($colors{$color}->[$j])) {
                    if ($colors{$color}->[$j] == $ARG[0]) {
                        return $j;
                    }
            }
        }
    }
}

# is_my_neighbor:
#
# $ARG[0] : Vertex ID.
# $ARG[1] : Vertex ID.
#
# Return TRUE is they are neighbor.
sub is_my_neighbor($$) {
    my ($here, $there) = @ARG;
    # checking in one way
    foreach (@{ $graph[$here] }) {
        if ($_ == $there) {
            return 1;
        }
    }
    0;
}


# is_bad_color:
#
# $ARG[0] : Color ID.
# $ARG[1] : Vertex ID.
#
# Supposing a COLOR, check if there are a neighbor guy with the same color.
# Return TRUE if it is not a good color
# NOTE: As colors[] is local for every thread then there aren't external vertex in the list.
sub is_bad_color($$) {
    my ($color, $node) = @ARG;
    foreach (@{$graph[$node]}) {
        return 1 if (get_colorID($_) == $color);
    }
    0;
}
# get_color:
#
# $ARG[0] : Vertex ID.
# For a given vertex return the best color, checking the neighbor guys.
#
# Return the selected Color ID.
sub get_color($)
{
    my ($node) = @ARG;
    # first option, try a USED color
    my $cols = @colors_defined;
    # first shuffle them a bit
    if ($cols > 1) {
        push(@colors_defined, splice(@colors_defined, 0, int(rand($cols))));
    }
    my %used =
        map {
            if (defined($colored_vertex{$_})) {
                ($colored_vertex{$_} => 1);
            } else {
                ();
            }
    } @{$graph[$node]};
    foreach (@colors_defined) {
        return $_ if (!defined($used{$_}));
    }
    # second option,tray a UN-USED color
    my $color;
    # looking for the first GOOD COLOR randonly
    # if we don't do that we could have local problems
  REDO:
    if (scalar @colors_defined >= ($color_palette + 1)) {
        print STDERR "using already all $color_palette colors. Augmenting.\n";
        ++$color_palette;
    }
    $color = int(rand($color_palette));
    goto REDO if (is_bad_color($color, $node));
    push @colors_defined, $color;
    $last_color_used = $color;
    return $last_color_used;
}

# similar than get_color just it uses "2" functions
sub get_color2($)
{
    my ($node) = @ARG;
    # first option, try a USED color
    my $cols = @colors_defined;
    # first shuffle them a bit
    if ($cols > 1) {
        push(@colors_defined, splice(@colors_defined, 0, int(rand($cols))));
    }
    my %used =
        map {
            if ($colored_vertex{$_}) {
                ($colored_vertex{$_} => 1);
            } else {
                ();
            }
    } @{$graph[$node]};
    foreach (@colors_defined) {
        return $_ if (!defined($used{$_}));
    }
    # second option,try a UN-USED color
    my $color;
    # looking for the first GOOD COLOR randonly
    # if we don't do that we could have local problems
  REDO:
    if (scalar @colors_defined >= ($color_palette + 1)) {
        print STDERR "using already all $color_palette colors. Augmenting.\n";
        ++$color_palette;
    }
    $color = int(rand($color_palette));
    goto REDO if (is_bad_color($color, $node));
    push @colors_defined, $color;
    $last_color_used = $color;
    return $last_color_used;
}
# communicate_color:
#
# $ARG[0] : Vertex.
# $ARG[1] : Color.
#
# Communicate vertex's using shared memory.
# That procedures must be re-written for a distributed environment.
#
sub communicate_coloring($$)
{
    $colored_vertex[$ARG[0]] = $ARG[1];
}

# Colors-Vertex to be sent
my %color_dispacher;
# Colors-Vertex to be receiv
my %color_receiver;

# send_colors:
#
# Send The information to others threads. At the moment i am using shared memory.
# TODO: Use sockets for distributed system.
# TODO: why is this called with the thread ID?
sub send_colors($)
{
    foreach (keys %color_dispacher)
    {
        $colored_vertex[$_] = $color_dispacher{$_};
    }
}

# receiv_colors:
#
# Receiv information to others threads using shared memory.
# TODO: Use sockets for distributed system.
# TODO: why is this called with the thread ID?
sub receiv_colors($)
{
    foreach (keys %color_receiver)
    {
        # wait for the remote thread
        while (!(defined($colored_vertex[$_]))){}
        $color_receiver{$_} = $colored_vertex[$_];
    }
}
#
# do_coloring:
#
# Procedure that runs in every thread and makes the local-coloring. The procedures with number "2" are used in phase 2
# of the algorithm. It is passing the range of the vertex that the thread has to coloring.
#
# $ARG[0]: Counter start.
# $ARG[1]: Counter end.
# $ARG[2]: Thread ID.
#
sub do_coloring($$$)
{
    # graph index
    my ($start_i, $end_i, $threadID) = @ARG;
    my @nodes = grep { defined($graph[$_]) } ($start_i .. $end_i);
    foreach my $node (
        sort {
            scalar @{$graph[$b]}
            <=>
                scalar @{$graph[$a]}
        } @nodes) {
        if (!has_color($node)) {
            my $color = get_color($node);
            # coloring the vertex
            color_node($color, $node);
            print  STDERR "Thread $threadID, coloring $node with color $color\n" if $check;
        }
        # Has the location been colored?
        # looking for the conections
        for my $neig (@{$graph[$node]}) {
            # only coloring on local vertex
            if (is_local($start_i, $end_i, $neig)) {
                # Has the location been colored?
                if  (!has_color($neig)) {
                    my $color = get_color($neig);
                    # coloring the vertex
                    color_node($color, $neig);
                    printf  STDERR "Thread $threadID, coloring %d with color %d\n", $neig, $color if $check;
                }
            } else {
                # we have a non-local element
                # we have to send this information in the next phase
                $color_dispacher{$node} = get_colorID($node);
                # we have to receiv this information in the next phase
                $color_receiver{$neig} = '';
            }
        }
    }

    # Phase 2 starts
    # we have to send and receiv information from the border
    #

    # send the information of color_dispacher
    send_colors($threadID);
    # receiv information from others threads
    receiv_colors($threadID);

    # store the boundary information into the local structures
    foreach my $node (keys %color_receiver) {
        color_node($color_receiver{$node}, $node);
    }
    # if a vertex is in a color_dispacher then it has a non-local guy
    foreach my $node (keys %color_dispacher) {
        my $cf;
        my $color = get_colorID($node);
        foreach my $neigh (@{ $graph[$node] }) {
            # we can only have problem with non-local guys
            if (
                (@{$graph[$node]} > @{$graph[$neigh]})
                && ($node < $neigh)
                && !is_local($start_i, $end_i, $neigh)
                && ($color == get_colorID($neigh))) {
                # we have a conflict, just one guy fix the conflict
                $cf = 1;
            }
        }
        # I have a conflict! recolor the vertex
        if ($cf) {
            my $ID = get_colorID2($node);
            # I need to remove it from the array
            uncolor_node($color, $ID);
            # get  a new color using external information
            $color = get_color($node);
            # save it
            color_node($color, $node);
            print STDERR "Thread $threadID, Recoloring $node with $color\n" if $check;
        }
    }
    # write the data to shared memory region
    # this procedure must be rewritten in a distributed environment
    foreach my $node (@nodes) {
        communicate_coloring($node, $colored_vertex{$node});
    }
}


#
# The following subroutines are used only for fix the conflic coloring.
# Just one guy can call to fix_conflicts()
#

# is_bad_color_main:
#
# $ARG[0] : Color ID.
# $ARG[1] : Vertex ID.
#
# Supposing a COLOR, check if there are a neighbor guy with the same color.
# Return TRUE if it is not a good color.
# Don't used with threads just for Main task.
#
sub is_bad_color_main($$)
{
    my ($color, $node) = @ARG;
    foreach (@{$graph[$node]}) {
        return 1 if ($colored_vertex[$_] == $color);
    }
    0;
}
# get_color_main:
#
# $ARG[0] : Vertex ID.
# For a given vertex return the best color, checking the neighbor guys.
#
# Return the selected Color ID. Don't call from threads just from main task.
#
sub get_color_main($)
{
    my ($node) = @ARG;
    # first option, try a USED color
    my $cols = @colors_defined;
    # first shuffle them a bit
    if ($cols > 1) {
        push(@colors_defined, splice(@colors_defined, 0, int(rand($cols))));
    }
    my %used =
        map {
            if (defined($colored_vertex[$_])) {
                ($colored_vertex[$_] => 1);
            } else {
                ();
            }
    } @{$graph[$node]};
    foreach (@colors_defined) {
        return $_ if (!defined($used{$_}));
    }
    # second option,tray a UN-USED color
  REDO:
    if (scalar @colors_defined >= ($color_palette + 1)) {
        print STDERR "using already all $color_palette colors. Augmenting.\n";
        ++$color_palette;
    }
    $last_color_used = int(rand($color_palette));
    # looking for the first GOOD COLOR randonly
    # if we don't do that we could have local problems
    goto REDO if (is_bad_color_main($last_color_used,$node));
    push @colors_defined, $last_color_used;
    return $last_color_used;
}

#
# fix_conflicts:
#
# Coloring Algorithm, phase 2.
# Just called from Main task.
#
sub fix_conflicts()
{
    # looking for the graph
    foreach my $i (keys %nom) {
        if (@{$graph[$i]}) {
            # it has connections ---> check coloring
            my $color1 = $colored_vertex[$i];
            # looking for in the neigboard tail
            foreach my $neig (@{ $graph[$i] }) {
                my $color2 = $colored_vertex[$neig];
                # same color --> conflic!
                if ($color1 == $color2) {
                    # 1 conflic per connection
                    if ($i > $neig) {
                        my $tr;
                        foreach (@{ $graph[$neig] }) {
                            if ($_ == $i) {
                                $tr = 1;
                                last;
                            }
                        }
                        #  neig --> i connection doesn't exist then we have to increment the conflict counter
                        if (!$tr) {
                                # change the color
                            $colored_vertex[$neig] = get_color_main($neig);
                        }
                    } else {
                        # change the color
                        $colored_vertex[$neig] = get_color_main($neig);
                    }
                }
            }
        }
    }
}

#
# The following subroutines are used only for the input and ouput and they aren't used for the algorithm
#
#

# check_conflicts:
#
# Count the number of conflicts
#
sub check_conflicts()
{
    my $conflicts_count = 0;
    # looking for the graph
    foreach my $i (keys %nom) {
        # it is a isolated guy, check coloring
        if (!@{$graph[$i]}) {
            if (!(defined($colored_vertex[$i]))) {
                print STDERR "Vertex $i isolated not colored!\n" if $check;
            }
        } else {
            # it has conections ---> check cloring
            my $color1 = $colored_vertex[$i];
            # looking for in the neigboard tail
            for my $neig (@{ $graph[$i] }) {
                my $color2 = $colored_vertex[$neig];
                # same color --> conflic!
                if ($color1 == $color2) {
                    # 1 conflic per conection
                    if ($i > $neig) {
                        my $tr;
                        for (@{ $graph[$neig] }) {
                            if ($_ == $i) {
                                $tr = 1;
                                last;
                            }
                        }
                        #  neig --> i connection doesn't exist then we have to increment the conflict counter
                        if (!$tr) {
                            printf STDERR "Conflic between %d and %d\n", $i, $neig if $check;
                                # change the color
                            $conflicts_count++;
                        }
                    } else {
                        printf STDERR "Conflic between %d and %d\n", $i, $neig if $check;
                        # change the color
                        $conflicts_count++;
                    }
                }
            }
        }
    }
    print STDERR "Number of conflicts ---> $conflicts_count\n" if $check;
}

# make_output:
#
# Show the graph coloring
#
sub make_output($)
{
    my ($fd) = @ARG;
    print $fd $graphhead;
    local $OUTPUT_FIELD_SEPARATOR = ", ";
    # a little patch to re-use coloreo.pl source, not very eficient but it is just for the output
    # colors[] is emply for the main task
    for (my $j = 0; $j <= $#colored_vertex; $j++) {
        if (defined($colored_vertex[$j])) {
            color_node($colored_vertex[$j], $j);
        }
    }
    my $colors = scalar keys %colors;
    print $fd "/* Total number of colors: $colors */\n";
    my $col = int(rand(0xFFFFFF));
    my @rgb = (($col >> 16) & 0xFF, ($col >> 8) & 0xFF, $col & 0xFF);
    my @dif = ((int(rand(0x40)) + 0x80), (int(rand(0x40)) + 0x80), (int(rand(0x40)) + 0x80));
    my $nj = 0;
    my %ncols;
    my %cols;
    foreach my $class (sort { $a <=> $b } keys %colors) {
        @{$colors{$class}} = sort { $a <=> $b } @{$colors{$class}};
        for (my $i = 0; $i < 3; ++$i) {
            $rgb[$i] += $dif[$i];
            $rgb[$i] &= 0xFF;
        }
        $ncols{$class} = $nj;
        $cols{$class} = sprintf("#%02X%02X%02X", "$rgb[0]", "$rgb[1]", "$rgb[2]");
        print $fd "/* ".scalar @{ $colors{$class} }." vertices with color $nj. Color $cols{$class}. */\n";
        ++$nj;
    }
    print $fd $graphlist;
    foreach my $node (sort { $a <=> $b } keys %nom) {
        if ($number) {
            # Or output with numbers
            $nom{$node}->{color} = $ncols{$colored_vertex[$node]};
        } else {
            $nom{$node}->{color} = "$cols{$colored_vertex[$node]}";
            $nom{$node}->{style} = "filled";
            $nom{$node}->{label} = "$ncols{$colored_vertex[$node]}";
        }
        print $fd "$node\t[";
        print $fd map { "$_=\"$nom{$node}->{$_}\"" } keys %{$nom{$node}};
        print $fd "]\n";
    }
    print $fd "}\n";
}

# make_graph:
#
# Read from dotty file and fill the graph structure.
#
sub make_graph()
{
    my $sp;
    # opening dot file
    open(my $in,  "<",  $ifile);
    $sp = "-[->]";
    while (<$in>)
    {
        # first look for the separator: " -- " or " -> "
        if (m/$sp/o) {
            my ($id, $neigh) = m/^\s*(\d+)\s*$sp\s*(\d+)/o;
            create_edge($id, $neigh);
            $graphlist .= $_;
            # node metadata, saving that information
        } elsif (m/^\s*(\d+)\s*(?:\[(.+)\])?/o) {
            my $id = $1;
            create_vertex($id);
            if (defined($2)) {
                my @ndata = split(/, /, $2);
                %{$nom{$id}} = map {
                    if (m/(\w+)\s*=\s*"([^"]+)"/o) {
                        ($1 => $2);
                    } else {
                        undef;
                    }
                } @ndata;
            }
        }  elsif (!m/}/o) {
            $graphhead .= $_;
        }
    }
    close $in;
    ## Eliminate eventual duplicates from the lists
    foreach my $nl (@graph) {
        my %tmp = map { $_ => 1 } @{$nl};
        @{$nl} = keys %tmp;
    }
}

#######################################################################
### real execution starts here


# open .dot file and fill the graph structure
make_graph;
# Vertex per threads
$VxperTh = int (($#graph+1) / $ths);

# Schedule the threads
if ($ths > 1) {
    print STDERR "running $ths parallel threads\n";
    for (my $j = 0; $j < $ths; $j++)
    {
        my $start_i = $j*$VxperTh;
        my $end_i   = ($start_i + $VxperTh)-1;
        # Am I the last?
        if (($j+1) == $ths)
        {
            if ((($#graph+1) % $ths) != 0)
            {
                $end_i = $end_i + (($#graph+1) % $ths);
            }
        }
        if ($j == $ths-1) {
            # do the last thread ourself
            do_coloring($start_i, $end_i, $j);
        } else {
            $t[$j] = Thread->new( \&do_coloring, $start_i, $end_i, $j);
        }
    }
    # wait for threads
    # run in parallel
    for (my $j = 0; $j < ($ths-1); $j++)
    {
        my $retval = $t[$j]->join();
    }
    # fix the problems
    fix_conflicts;
} else {
    print STDERR "only 1 thread, doing it sequentially\n";
    do_coloring(0, (scalar keys %nom)-1, 0);
}

# show the coloring in dotty format
if ($ofile) {
    open(my $fd, ">$ofile") || die "unable to write to file $ofile";
    make_output($fd);
} else {
    make_output(\*STDOUT);
}
# internal runtime to check the coloring
check_conflicts if $check;

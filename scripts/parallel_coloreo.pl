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
my @colors;
# List of colors already used
my @colors_defined;

# vertex' names
my @nom;

# colored_vertex[X] holds the color of vertex X.
my @colored_vertex : shared;

# graph[X] is a list of vertices that are connected with X.
# It is filled when the application starts.
my @graph;

# assignment uses a simple counter
# WARNING that variable is used by the threads but it not protected!
my $last_color_used = 0;

# Thread List
my @t;

# parameter passing
my $ifile = '';
my $undirected = '';
my $number = '';
my $check = '';
my $ths = 0;
my $VxperTh;


# the information as found in the graph
my $graphhead = "";

# Maximun number of color used, at the moment this paramter is fixed.
# Using: < Delta + 1 as a limited.
# In matrix case 15 is the Max degree.
# In the worse case it uses $color_palette colors.
# TODO: transform this into a parameter
my $color_palette = 18;

#
# is_local:
#
# $_[0]: Counter start.
# $_[1]: Counter end.
# $_[2]: Vertex ID.
#
# Return true if the thread has the Vertex.
# TODO: This procedure must be re-written in a distributed environment.
#
sub is_local {
    if (($_[2] <= $_[1]) && ($_[0] <= $_[2])) {
        return 1;
    }
}
# has_color:
#
# $_[0]: vertex ID.
#
# return true if a vertex has been colored
#
sub has_color
{
    for (my $i = 0; $i <= $#colors; $i++)
    {
        for (my $j = 0; $j <= $#{ $colors[$i] }; $j++)
        {
            if ($colors[$i]->[$j] == $_[0])
            {
                return 1;
            }
        }
    }
}

# get_colorID:
#
# $_[0]: vertex ID.
#
# return vertex's color
#
sub get_colorID
{
    for (my $i = 0; $i <= $#colors; $i++)
    {
        if (defined($colors[$i]))
        {
            for (my $j = 0; $j <= $#{ $colors[$i] }; $j++)
            {
                if (defined($colors[$i]->[$j]))
                {
                    if ($colors[$i]->[$j] == $_[0])
                    {
                        return $i;
                    }
                }
            }
        }
    }
}
# similar than get_colorID, this one returns $j (position into the color queue) while the other return $i (color)
sub get_colorID2
{
    for (my $i = 0; $i <= $#colors; $i++)
    {
        if (defined($colors[$i]))
        {
            for (my $j = 0; $j <= $#{ $colors[$i] }; $j++)
            {
                if (defined($colors[$i]->[$j]))
                {
                    if ($colors[$i]->[$j] == $_[0])
                    {
                        return $j;
                    }
                }
            }
        }
    }
}

# is_my_neighbor:
#
# $_[0] : Vertex ID.
# $_[1] : Vertex ID.
#
# Return TRUE is they are neighbor.
#
sub is_my_neighbor
{
    # checking in one way
    for (my $j = 0; $j <= $#{ $graph[$_[0]] }; $j++)
    {
        if ($graph[$_[0]]->[$j] == $_[1])
        {
            return 1;
        }
    }
    # checking in the other way
    for (my $j = 0; $j <= $#{ $graph[$_[1]] }; $j++)
    {
        if ($graph[$_[1]]->[$j] == $_[0])
        {
            return 1;
        }
    }
}
# similar that is_my_neighbor() but in this case just check in one direction cause
# the other direction can be in other processor.
sub is_my_neighbor2
{
    # checking in one way
    for (my $j = 0; $j <= $#{ $graph[$_[0]] }; $j++)
    {
        if ($graph[$_[0]]->[$j] == $_[1])
        {
            return 1;
        }
    }
}
# is_good_color:
#
# $_[0] : Color ID.
# $_[1] : Vertex ID.
#
# Supposing a COLOR, check if there are a neighbor guy with the same color.
# Return TRUE if it is not a good color
# NOTE: As colors[] is local for every thread then there aren't external vertex in the list.
#
sub is_good_color
{
    for (my $j = 0; $j <= $#{ $colors[$_[0]] }; $j++)
    {
        if (defined($colors[$_[0]]->[$j]))
        {
            if (is_my_neighbor($_[1],$colors[$_[0]]->[$j]))
            {
                return 1;
            }
        }
    }
}
# similar that is_good_color() the only diference is is_my_neighbor2()
sub is_good_color2
{
    for (my $j = 0; $j <= $#{ $colors[$_[0]] }; $j++)
    {
        if (defined($colors[$_[0]]->[$j]))
        {
            if (is_my_neighbor2($_[1],$colors[$_[0]]->[$j]))
            {
                return 1;
            }
        }
    }
}
# get_color:
#
# $_[0] : Vertex ID.
# For a given vertex return the best color, checking the neighbor guys.
#
# Return the selected Color ID.
sub get_color
{
    # first option, tray a USED color
    foreach (@colors_defined)
    {
        if (!(is_good_color($_,$_[0])))
        {
            return $_;
        }
    }
    # second option,tray a UN-USED color
    $last_color_used = int(rand($color_palette));
    # looking for the first GOOD COLOR randonly
    # if we don't do that we could have local problems
    while (is_good_color($last_color_used,$_[0]))
    {
        $last_color_used = int(rand($color_palette))
    }
    push @colors_defined, $last_color_used;
    return $last_color_used;
}

# similar than get_color just it uses "2" functions
sub get_color2
{
    # first option, tray a USED color
    foreach (@colors_defined)
    {
        if (!(is_good_color2($_,$_[0])))
        {
            return $_;
        }
    }
    # second option,tray a UN-USED color
    $last_color_used = int(rand($color_palette));
    # looking for the first GOOD COLOR randonly
    # if we don't do that we could have local problems
    while (is_good_color2($last_color_used,$_[0]))
    {
        $last_color_used = int(rand($color_palette))
    }
    push @colors_defined, $last_color_used;
    return $last_color_used;
}
# comunicate_color:
#
# $_[0] : Vertex.
# $_[1] : Color.
#
# Comunicate vertex's using shared memory.
# That procedures must be re-written for a distributed environment.
#
sub comunicate_coloring
{
    $colored_vertex[$_[0]] = $_[1];
}

# Colors-Vertex to be sent
my @color_dispacher;
# Colors-Vertex to be receiv
my @color_receiver;

# send_colors:
#
# Send The information to others threads. At the moment i am using shared memory.
# TODO: Use sockets for distributed system.
sub send_colors
{
    for (my $i = 0; $i <= $#color_dispacher; $i++)
    {
        if (defined($color_dispacher[$i]))
        {
            $colored_vertex[$i] = $color_dispacher[$i];
        }
    }
}

# receiv_colors:
#
# Receiv information to others threads using shared memory.
# TODO: Use sockets for distributed system.
sub receiv_colors
{
    for (my $i = 0; $i <= $#color_receiver; $i++)
    {
        if (defined($color_receiver[$i]))
        {
            # wait for the remote thread
            while (!(defined($colored_vertex[$i]))){}
            $color_receiver[$i] = $colored_vertex[$i];
        }
    }
}
#
# do_coloring:
#
# Procedure that runs in every thread and makes the local-coloring. The procedures with number "2" are used in phase 2
# of the algorithm. It is passing the range of the vertex that the thread has to coloring.
#
# $_[0]: Counter start.
# $_[1]: Counter end.
# $_[2]: Thread ID.
#
sub do_coloring
{
    # graph index
    my $start_i = $_[0];
    my $end_i = $_[1];
    my $color_hd = '';
    my $color ='';
    # each graph[X] has a list of connections from vertex X to others guys
    for (my $i = $start_i; $i <= $end_i; $i++)
    {
        # If the element was defined then I can work with it
        # we don't work with solitary guys here
        if ( (defined($graph[$i])) && ($graph[$i]->[0] != -1) )
        {
            # Has the location been colored?
            if (!(has_color($i)))
            {
                $color = get_color($i);
                $color_hd = $color;
                # coloring the vertex
                push @{ $colors[$color] }, $i;
                print  STDERR "Thread $_[2], coloring $i with color $color\n" if $check;
            }
            # looking for the conections
            for (my $j = 0; $j <= $#{ $graph[$i] }; $j++)
            {
                # only coloring on local vertex
                if (is_local($start_i,$end_i,$graph[$i]->[$j]))
                {
                    # Has the location been colored?
                    if  (!(has_color($graph[$i]->[$j])))
                    {
                        my $color = get_color($graph[$i]->[$j]);
                        # coloring the vertex
                        push @{ $colors[$color] }, $graph[$i]->[$j];
                        printf  STDERR "Thread $_[2], coloring %d with color %d\n",$graph[$i]->[$j],$color if $check;
                    }
                } else
                    # we have a non-local element
                {
                    # we have to send this information in the next phase
                    $color_dispacher [$i] = get_colorID($i);
                    # we have to receiv this information in the next phase
                    $color_receiver [$graph[$i]->[$j]] = '';
                }
            }
        }
    }
    # Looking for a solitary vertex
    # It importants to coloring now because it is not always a solitary vertex is a not colored guy
    #
    for (my $i = $start_i; $i <= $end_i; $i++)
    {
        if (defined($graph[$i]))
        {
            if ( ($graph[$i]->[0] == -1) && (!(has_color($i))) )
            {
                my $color = get_color($i);
                push @{ $colors[$color] }, $i;
            }
        }
    }

    # Phase 2 starts
    # we have to send and receiv information from the border
    #

    # send the information of color_dispacher
    send_colors($_[2]);
    # receiv information from others threads
    receiv_colors($_[2]);

    # if a vertex is in a color_dispacher then it has a non-local guy
    for (my $i = 0; $i <= $#color_dispacher; $i++)
    {
        if (defined($color_dispacher[$i]))
        {
            my $cf = 0;
            # we have the vertex in $i
            for (my $j = 0; $j <= $#{ $graph[$i] }; $j++)
            {
                # we can only have problem with non-local guys
                if (!(is_local($start_i,$end_i,$graph[$i]->[$j])))
                {
                    $color = $color_receiver[$graph[$i]->[$j]];
                    # I need that information to solve a future conflict
                    push @{ $colors[$color] }, $graph[$i]->[$j];
                    # we have a conflict, just one guy fix the conflict
                    if ( ($color_dispacher[$i] == $color) && ($i>$graph[$i]->[$j]) ) {$cf=1}
                }
            }
            # I have a conflict! recolor the vertex
            if ($cf==1)
            {
                $color = $color_dispacher[$i];
                my $ID = get_colorID2($i);
                # I need to remove it from the aray
                delete $colors[$color]->[$ID];
                # get  a new color using external information
                $color = get_color2($i);
                # save it
                push @{ $colors[$color] }, $i;
                print STDERR "Thread $_[2], Recoloring $i with $color\n" if $check;
            }
        }
    }
    # write the data to shared memory region
    # this procedure must be rewritten in a distributed environment
    for (my $i = 0; $i <= $#colors; $i++)
    {
        if (defined($colors[$i]))
        {
            for (my $j = 0; $j <= $#{ $colors[$i] }; $j++)
            {
                if (defined($colors[$i]->[$j]))
                {
                    my $vertex = $colors[$i]->[$j];
                    # only send the information about local coloring
                    if (is_local($start_i,$end_i,$vertex))
                    {
                        comunicate_coloring($vertex,$i);
                    }
                }
            }
        }
    }
}


#
# The following subrutines are used only for fix the conflic coloring.
# Just one guy can call to fix_conflics()
#

# is_good_color_main:
#
# $_[0] : Color ID.
# $_[1] : Vertex ID.
#
# Supposing a COLOR, check if there are a neighbor guy with the same color.
# Return TRUE if it is not a good color.
# Don't used with threads just for Main task.
#
sub is_good_color_main
{
    for (my $i = 0; $i <= $#colored_vertex; $i++)
    {
        if (defined($colored_vertex[$i]))
        {
            # a guy with same color
            if ($colored_vertex[$i] == $_[0])
            {
                # if it is a neighbor we have a conflic
                if (is_my_neighbor($i,$_[1])) {return 1;}
            }
        }
    }
}
# get_color_main:
#
# $_[0] : Vertex ID.
# For a given vertex return the best color, checking the neighbor guys.
#
# Return the selected Color ID. Don't call from threads just from main task.
#
sub get_color_main
{
    # first option, tray a USED color
    foreach (@colors_defined)
    {
        if (!(is_good_color_main($_,$_[0])))
        {
            return $_;
        }
    }
    # second option,tray a UN-USED color
    $last_color_used = int(rand($color_palette));
    # looking for the first GOOD COLOR randonly
    # if we don't do that we could have local problems
    while (is_good_color_main($last_color_used,$_[0]))
    {
        $last_color_used = int(rand($color_palette))
    }
    push @colors_defined, $last_color_used;
    return $last_color_used;
}

#
# fix_conflics:
#
# Coloring Algorithm, phase 2.
# Just called from Main task.
#
sub fix_conflics
{
    # looking for the graph
    for (my $i = 0; $i <= $#graph; $i++)
    {
        if (defined($graph[$i]))
        {
            if (!($graph[$i]->[0] == -1))
            {
                # it has conections ---> check cloring
                my $color1 = $colored_vertex[$i];
                # looking for in the neigboard tail
                for (my $j = 0; $j <= $#{ $graph[$i] }; $j++)
                {
                    my $neig = $graph[$i]->[$j];
                    my $color2 = $colored_vertex[$neig];
                    # same color --> conflic!
                    if ($color1 == $color2)
                    {
                        # 1 conflic per conection
                        if ($i > $neig)
                        {
                            my $tr = 0;
                            for (my $l = 0; $l <= $#{ $graph[$neig] }; $l++)
                            {
                                if ($graph[$neig]->[$l] == $i)
                                {
                                    $tr = 1;
                                }
                            }
                            #  neig --> i connection doesn't exist then we have to increment the conflict counter
                            if (!($tr == 1))
                            {
                                # change the color
                                $colored_vertex[$neig] = get_color_main($neig);
                            }
                        } else
                        {
                            # change the color
                            $colored_vertex[$neig] = get_color_main($neig);
                        }
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
# Count the number of conflics
#
sub check_conflicts
{
    my $conflics_count = 0;
    # looking for the graph
    for (my $i = 0; $i <= $#graph; $i++)
    {
        if (defined($graph[$i]))
        {
            # it is a solitary guy, check coloring
            if ($graph[$i]->[0] == -1)
            {
                if (!(defined($colored_vertex[$i])))
                {
                    print STDERR "Vertex $i solitary not colored!\n" if $check;
                }
            }
            else
            {
                # it has conections ---> check cloring
                my $color1 = $colored_vertex[$i];
                # looking for in the neigboard tail
                for (my $j = 0; $j <= $#{ $graph[$i] }; $j++)
                {
                    my $neig = $graph[$i]->[$j];
                    my $color2 = $colored_vertex[$neig];
                    # same color --> conflic!
                    if ($color1 == $color2)
                    {
                        # 1 conflic per conection
                        if ($i > $neig)
                        {
                            my $tr = 0;
                            for (my $l = 0; $l <= $#{ $graph[$neig] }; $l++)
                            {
                                if ($graph[$neig]->[$l] == $i)
                                {
                                    $tr = 1;
                                }
                            }
                            #  neig --> i connection doesn't exist then we have to increment the conflict counter
                            if (!($tr == 1))
                            {
                                printf STDERR "Conflic between %d and %d\n", $i, $graph[$i]->[$j] if $check;
                                # change the color
                                $conflics_count++;
                            }
                        } else
                        {
                            printf STDERR "Conflic between %d and %d\n", $i, $graph[$i]->[$j] if $check;
                            # change the color
                            $conflics_count++;
                        }
                    }
                }
            }
        }
    }
    print STDERR "Number of conflics ---> $conflics_count\n" if $check;
}

# make_output:
#
# Show the graph coloring using STDOUT
#
sub make_output
{
    print $graphhead;
    local $OUTPUT_FIELD_SEPARATOR = ", ";
    # a little patch to re-use coloreo.pl source, not very eficient but it is just for the output
    # colors[] is emply for the main task
    for (my $j = 0; $j <= $#colored_vertex; $j++)
    {
        if (defined($colored_vertex[$j]))
        {
            push @{ $colors[$colored_vertex[$j]] }, $j;
        }
    }
    # then we can use colors[] again as in coloreo.pl
    for (my $j = 0; $j <= $#colors; $j++)
    {
        if (defined($colors[$j]))
        {
            # color random combination
            my $red   =   int(rand(255));
            my $green =   int(rand(255));
            my $blue  =   int(rand(255));
            foreach my $node ( @{ $colors[$j] } )
            {
                if (!$nom[$node]) {
                    my %ndata;
                    $nom[$node] = \%ndata;
                }
                # output colors
                if (!($number))
                {         # if we don't have metadata, we don't add it
                    $nom[$node]->{color} = sprintf("#%2x%2x%2x", $red ,$green ,$blue);
                    $nom[$node]->{style} = "filled";
                }
                # Or output with numbers
                else
                {
                    $nom[$node]->{color} = $j;
                }
                print "$node [";
                print map { "$_=\"$nom[$node]->{$_}\"" } keys %{$nom[$node]};
                print "]\n";
            }
        }
    }
    print "}\n";
}

# make_graph:
#
# Read from dotty file and fill the graph structure.
#
sub make_graph
{
    my $sp;
    # opening dot file
    open(my $in,  "<",  $ifile);
    # checking the input format
    if ($undirected)
    {
        $sp = "--";
    } else {
        # default is direct
        $sp = "->";
    }
    while (<$in>)
    {
        # first look for the separator: " -- " or " -> "
        if (m/$sp/o)
        {
            my @temp = m/^(\d+)\s*$sp\s*(\d+)/o;
            # make the graph
            # node source ---> node destination
            push @{ $graph[$temp[0]] }, $temp[1];
            if ($undirected) {push @{ $graph[$temp[1]] }, $temp[0];}
            $graphhead .= $_;
            # node metadata, saving that information
        } elsif (m/\s*(\d+)\s*\[(.+)\]/o)
        {
            my $id = $1;
            my @ndata = split(/, /, $2);
            my %ndata = map {
                if (m/(\w+)\s*=\s*"([^"]+)"/o) {
                    ($1 => $2);
                } else {
                    undef;
                }
            } @ndata;
            $nom[ $id ]  = \%ndata;
            #$graphhead .= $_;
        }  elsif (!m/}/o) {
            $graphhead .= $_;
        }
    }
    close $in;
    # we have to look for solitary guys
    for (my $i = 0; $i <= $#graph; $i++)
    {
        if (defined($graph[$i]))
        {
            for (my $j = 0; $j <= $#{ $graph[$i] }; $j++)
            {
                my $vertex_sol = $graph[$i]->[$j];
                # we have conection in one way so we must to defined the vertex
                if (!(defined($graph[$vertex_sol])))
                {
                    push @{ $graph[$vertex_sol] }, -1;
                }
            }
        }
    }
}
# getting the parameters
if ($#ARGV == -1)
{
    print "Usage: $PROGRAM_NAME [Parameters] [file]\n";
    exit;
}
GetOptions ('number'=>\ $number,'undirected'=>\ $undirected, 'check'=>\ $check, 'thread=i' =>\ $ths);
$ifile = $ARGV[0];
# default number of threads
$ths = 2 if ($ths eq 0);
# open .dot file and fill the graph structure
make_graph;
# Vertex per threads
$VxperTh = int (($#graph+1) / $ths);
# Scheduling the threads
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
    $t[$j] = Thread->new( \&do_coloring, $start_i, $end_i,$j);
}
# wait for threads
# run in parallel
for (my $j = 0; $j < $ths; $j++)
{
    my $retval = $t[$j]->join();
}
# fix the problems
fix_conflics;
# show the coloring in dotty format
make_output;
# internal runtime to check the coloring
check_conflicts if $check;

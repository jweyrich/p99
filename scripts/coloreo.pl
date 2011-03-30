#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- mode: perl; coding: utf-8 -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl
#
#
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
# Graph coloring using "list coloring" algoritm. The number of colors
# used is bounded from above by < DELTA + 1 being DELTA the maximum
# degree.
#
# Usage: coloreo.pl [file] [Parameters]
#
# Notes:
#
# - [file] is in .dot format, "label" sentences are used for naming
#   the vertex but they aren't necessary, comments and other kind of
#   information is ignored.
# - The vertex must be identify using numbers from "0" to "N".
#
# Example:
# > perl coloreo.pl gen.dot  // Coloring gen.dot, using direct format and outputing using colours in STDOUT

use English;
use strict;
use Getopt::Long;


my @nom;

# colors[X] is an array which every entry is a list of vertex that they are coloring with X colour.
# It is filled in running time
my @colors;

# graph[X] is an array which every entry is a list of vertex that they connected with X vertex.
# It is filled when the application starts
my @graph;

# assignment uses a simple counter
my $last_color_used = 0;

# parameter passing
my $ifile = '';
my $ind = '';
my $number = '';
my $check = '';


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
   for (my $j = 0; $j <= $#{ $colors[$i] }; $j++)
    {
     if ($colors[$i]->[$j] == $_[0])
     {
      return $i;
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

# is_good_color:
#
# $_[0] : Color ID.
# $_[1] : Vertex ID.
#
# For a given color and vertex, check if its neighbors has the same color.
# Return TRUE if it IS NOT a good color.
#
sub is_good_color
 {
  for (my $j = 0; $j <= $#{ $colors[$_[0]] }; $j++)
     {
      if (is_my_neighbor($_[1],$colors[$_[0]]->[$j])){return 1;}
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
  for (my $j = 0; $j <= $last_color_used; $j++)
   {
    # we found a good color, return with it
    if (!(is_good_color($j,$_[0]))) {return $j;}
   }
 $last_color_used++,
 return $last_color_used;
 }

#
# The following subrutines are used only for the input and ouput and they aren't used for the algoritm
#
#

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
   if ($ind) 
    {
     $sp = " -- "; 
    } else {
    # default is direct
     $sp = " -> ";
    }  
   while (<$in>)
    {
    # first look for the separator: " -- " or " -> " -> "
    if (!(index($_,$sp)==-1))
     {
      my @temp;
      # have we got comments?
      if (!(index($_,"//")==-1))
       {
        # yes, remove it
        my @comment = split ("//",$_);
        @temp = split ($sp,$comment[0]);
       } else {
        @temp = split ($sp,$_);
       }
      # make the graph
      # node source ---> node destination
      push @{ $graph[$temp[0]] }, $temp[1];
      if ($ind) {push @{ $graph[$temp[1]] }, $temp[0];}
      # node metadata, saving that information
     } elsif (!(index($_,"label")==-1))
       {
          # node name
          my @name = split ('"',$_);
          # node ID
          my @ID = split (' ',$_);
          # saving the pairs "ID" --> "NAME
          $nom[ $ID[0] ]  = $name[1];
       }
    }
   close $in;
  }

# make_output:
#
# Show the graph coloring using STDOUT
#
sub make_output
 {
  my $data = "";
  # store the data
  open(my $out, "<",  $ARGV[0]);
  while (<$out>)
   {
    #  char "}" and node metadata will be ignored, rest of information will be saved
    if ((index($_,"}")==-1) && (index($_,"label")==-1)) {$data = $data.$_;}
   }
  close $out;
  # store again, removing the last character
  print $data;
  for (my $j = 0; $j <= $#colors; $j++)
   {
    # color random combination
    my $red   =   int(rand(255));
    my $green =   int(rand(255));
    my $blue  =   int(rand(255));
    for (my $i = 0; $i <= $#{ $colors[$j] }; $i++)
     {
        # output colors
       if (!($number))
        {  # if we dont have metadata, we dont add it
           if ( $nom[ $colors[$j]->[$i] ] )
            {
             printf "%d [color=\"#%2x%2x%2x\", label=\"%s\", style=filled]\n",$colors[$j]->[$i] ,$red ,$green ,$blue ,$nom[ $colors[$j]->[$i] ];
            } else
            {
              # little bug in perl
              if($colors[$j]->[$i] == 0)
               { printf "%d [color=\"#%2x%2x%2x\", label=\"0\", style=filled]\n",$colors[$j]->[$i],$red,$green,$blue; }
              else
               { printf "%d [color=\"#%2x%2x%2x\", style=filled]\n",$colors[$j]->[$i] ,$red ,$green ,$blue; }
            }
        }
        # Or output with numbers
        else
        {
          # if we dont have metadata, we dont add it
           if ( $nom[ $colors[$j]->[$i] ] )
            {
             printf "%d [color=\"%d\", label=\"%s\"]\n",$colors[$j]->[$i], $j, $nom[ $colors[$j]->[$i] ];
            } else
            {
             # little bug in perl
             if($colors[$j]->[$i] == 0)
              { printf "%d [color=\"%d\", label=\"0\"]\n",$colors[$j]->[$i], $j; }
             else
              { printf "%d [color=\"%d\"]\n",$colors[$j]->[$i], $j; }
            }
        }
     }
   }
  print "}\n";
 }

# check_coloring:
#
# Check if the coloring is correctly.
#
sub check_coloring
 {
 # looking for the graph
 for (my $i = 0; $i <= $#graph; $i++)
  {
   for (my $j = 0; $j <= $#{ $graph[$i] }; $j++)
    {
    if (get_colorID($i) == get_colorID($graph[$i]->[$j]))
     {
      print "// Checking coloring ... Fault\n";
      return;
     }
    }
  }
  print "// // Checking coloring ... Well done\n";
 }


# getting the parameters
if ($#ARGV == -1) 
 {
  print "Usage: coloreo.pl [file] [Parameters]\n";
  exit;
 }
$ifile = $ARGV[0];
GetOptions ('number'=>\ $number,'indirect'=>\ $ind, 'check'=>\ $check);

# open .dot file and fill the graph structure
make_graph;

# each graph[X] has a list of connections from vertex X to others guys
for (my $i = 0; $i <= $#graph; $i++)
 {
  # If the element was defined then I can work with it
  if (defined($graph[$i]))
   {
    # Has the location been colored?
    if (!(has_color($i)))
     {
      my $color = get_color($i);
      # coloring the vertex
      push @{ $colors[$color] }, $i;
     }
    # looking for the conections
    for (my $j = 0; $j <= $#{ $graph[$i] }; $j++)
     {
      # Has the location been colored?
      if  (!(has_color($graph[$i]->[$j])))
       {
        my $color = get_color($graph[$i]->[$j]);
        # coloring the vertex
        push @{ $colors[$color] }, $graph[$i]->[$j];
       }
     }
   }
 }

# show the coloring in dotty format
make_output;

# simple runtime to check if the coloring was Ok
if ($check) 
 {
  check_coloring;
 }

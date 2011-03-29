#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- mode: perl; coding: utf-8 -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl
#
#
# Except of parts copied from previous work and as explicitly stated below,
# the authors and copyright holders for this work are as follows:
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
# coloreo.pl
#
# Graph coloring using "list coloring" algoritm. The number of colors used is bounded from above by  < DELTA + 1
# being DELTA the maximum degree. 
#
# Use coloreo.pl [file] [Input format] [Output format]
#
# [file] is a dot file  
# [Input format] is -d for direct graph or -i indirect graph.
# [Output format] is -c for colors or -n for number identification.
#
# Notes: 
#
# - [file] is in .dot format, "label" sentences are used for naming the vertex but they aren't necessary, comments and other kind of
#   information is ignored. 
# - The vertex must be identify using numbers from "0" to "N".
# - [file] is used as the output and it is added the coloring. 
#
# Example:
# > perl coloreo.pl gen.dot -d -c  // Coloring gen.dot, using direct format and outputing using colours
#
#
# 17.03.2011. First version by Matias E. Vara, matiasvara@hotmail.com
#
#
#
# !/usr/bin/perl

# has_color:
#
# $_[0]: vertex ID.  
#
# return true if a vertex has been colored
# 
sub has_color
 {
  local ($j,$i);
  for ($i = 0; $i <= $#colors; $i++) 
   {
   for ($j = 0; $j <= $#{ $colors[$i] }; $j++) 
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
  local ($j,$i);
  for ($i = 0; $i <= $#colors; $i++) 
   {
   for ($j = 0; $j <= $#{ $colors[$i] }; $j++) 
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
  local ($j);
  # checking in one way
  for ($j = 0; $j <= $#{ $graf[$_[0]] }; $j++) 
   { 
    if ($graf[$_[0]]->[$j] == $_[1])
     {
      return 1;
     }
   }
  # checking in the other way
  for ($j = 0; $j <= $#{ $graf[$_[1]] }; $j++) 
   { 
    if ($graf[$_[1]]->[$j] == $_[0])
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
  local ($j);
  for ($j = 0; $j <= $#{ $colors[$_[0]] }; $j++) 
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
  local ($j);
  for ($j = 0; $j <= $last_color_used; $j++)
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

# make_graf:
#
# Read from dotty file and fill the graph structure.
#
sub make_graf
  {
   local($sp,$name,$ID,$comment,$temp);
   # opening dot file
   open(my $in,  "<",  $ARGV[0]);
   if (!(index($ARGV[1],"-d")==-1)) {$sp = " -> "; } else {$sp = " -- ";}
   while (<$in>) 
    {
    # first look for the separator: " -- " or " -> " -> "
    if (!(index($_,$sp)==-1))
     {
      # have we got comments?
      if (!(index($_,"//")==-1)) 
       {
        # yes, remove it
        @comment = split ("//",$_);
        @temp = split ($sp,$comment[0]);
       }else{ @temp = split ($sp,$_);}
      # make the graf
      # node source ---> node destination
      push @{ $graf[$temp[0]] }, $temp[1];
      if (!(index($ARGV[1],"-i")==-1)) {push @{ $graf[$temp[1]] }, $temp[0];}
      # node metadata, saving that information
     } elsif (!(index($_,"label")==-1)) 
       {
          # node name
          @name = split ('"',$_);
          # node ID
          @ID = split (' ',$_);
          # saving the pairs "ID" --> "NAME 
          $nom[ $ID[0] ]  = $name[1];
       }
    }
   close $in;
  }

# make_output:
# 
# Add the coloring to the dotty file.
#
sub make_output
 {
  local ($j,$i,$data,$node);
  # store the data
  open(my $out, "<",  $ARGV[0]);  
  while (<$out>)
   {
    #  char "}" and node metadata will be ignored, rest of information will be saved
    if ((index($_,"}")==-1) && (index($_,"label")==-1)) {$data = $data.$_;}
   }
  close $out;
  open(my $out, ">",  $ARGV[0]);
  # store again, removing the last character
  print $out $data;
  for ($j = 0; $j <= $#colors; $j++) 
   {
    # color random combination
    $red   =   int(rand(255));
    $green =   int(rand(255));
    $blue  =   int(rand(255));
    for ($i = 0; $i <= $#{ $colors[$j] }; $i++) 
     {
        # output colors
        # TODO: Node "0" is not identificable
       if (!(index($ARGV[2],"-c")==-1)) 
        {  # if we dont have metadata, we dont add it
           if ( $nom[ $colors[$j]->[$i] ] ) 
            {
             printf $out "%d [color=\"#%2x%2x%2x\", label=\"%s\", style=filled]\n",$colors[$j]->[$i] ,$red ,$green ,$blue ,$nom[ $colors[$j]->[$i] ];
            } else
            { 
              # little bug in perl
              if($colors[$j]->[$i] == 0)
               { printf $out "%d [color=\"#%2x%2x%2x\", label=\"0\", style=filled]\n",$colors[$j]->[$i],$red,$green,$blue; } 
              else 
               { printf $out "%d [color=\"#%2x%2x%2x\", style=filled]\n",$colors[$j]->[$i] ,$red ,$green ,$blue; }
            }         
        } 
        # Or output ID
        elsif (!(index($ARGV[2],"-n")==-1))
        { 
          # if we dont have metadata, we dont add it
           if ( $nom[ $colors[$j]->[$i] ] ) 
            {
             printf $out "%d [color=\"%d\", label=\"%s\"]\n",$colors[$j]->[$i], $j, $nom[ $colors[$j]->[$i] ];
            } else
            { 
             # little bug in perl
             if($colors[$j]->[$i] == 0) 
              { printf $out "%d [color=\"%d\", label=\"0\"]\n",$colors[$j]->[$i], $j; } 
             else 
              { printf $out "%d [color=\"%d\"]\n",$colors[$j]->[$i], $j; }
            }  
        }
     }
   }
  print $out "}\n";
  close $out;   
 }

# check_coloring:
# 
# Check if the coloring is correctly.
#
sub check_coloring
 {
 local ($i,$j);
 # looking for the graf
 for ($i = 0; $i <= $#graf; $i++) 
  {
   for ($j = 0; $j <= $#{ $graf[$i] }; $j++) 
    { 
    if (get_colorID($i) == get_colorID($graf[$i]->[$j]))
     {
      print "Bad coloring, review the algoritm stupid!\n";
      return;
     }
    }
  }
  print "Good boy!\n";
 }


#
# Here the game starts
#
#

# colors[X] is an array which every entry is a list of vertex that they are coloring with X colour.
# It is filled in running time
my $colors = 0;
# graf[X] is an array which every entry is a list of vertex that they connected with X vertex.
# It is filled when the application starts 
my $graf = 0; 

if ($#ARGV < 2) 
 { 
   print "Use coloreo.pl [file] [Input format] [Output format]\n";
   print "where: [file = .dot], [Input format = -d or -i], [Output format = -c or -n]\n";  
   exit;
 }
# open .dot file and fill the graph structure
make_graf;

# assignation uses a simple counter
$last_color_used = 0;

# each graf[X] has a list of connections from vertex X to others guys 
# we start with Node 0.
for ($i = 0; $i <= $#graf; $i++) 
 {
  # If the element was defined then I can work with it
  if (!($graf[$i] == undef))
   {
    # Has the location been colored?
    if (!(has_color($i))) 
     {
      $color = get_color($i);
      # coloring the vertex
      push @{ $colors[$color] }, $i;
     }  
    # looking for the conections
    for ($j = 0; $j <= $#{ $graf[$i] }; $j++) 
     { 
      # Has the location been colored?
      if  (!(has_color($graf[$i]->[$j])))
       { 
        $color = get_color($graf[$i]->[$j]);
        # coloring the vertex
        push @{ $colors[$color] }, $graf[$i]->[$j]; 
       }
     }
   }
 }

# simple runtime to check if the coloring was Ok
check_coloring;

# save the coloring graf to file
make_output;


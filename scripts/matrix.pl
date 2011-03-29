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

# Usage: matrix.pl [rows] [colums]
#
# [rows] is the rows of the  big matrix.
# [colums] is the number of colums of the big matrix.
#
# Make a graph corresponding of the Matrix regions connections. Take
# into consideration that every (i,j) is a submatrix and it has nine
# regions called: N, S, W, E, NW, NE, SE, SW and the central point.

if ($#ARGV < 1)
 {
   print "Usage matrix.pl [rows] [colums]\n";
   exit;
 }

$node_count = 0;

$col = $ARGV[1];
$col--;
$row = $ARGV[0];
$row--;

# creating the nodes identificator
for ($i = 0; $i <= $row; $i++)
 {
  for ($j = 0; $j <= $col; $j++)
   {
    $hash_nodes{ $i.$j } = $node_count++;
    $hash_nodes{ $i.$j.'N' } = $node_count++;
    $hash_nodes{ $i.$j.'S' } = $node_count++;
    $hash_nodes{ $i.$j.'W' } = $node_count++;
    $hash_nodes{ $i.$j.'E' } = $node_count++;
    $hash_nodes{ $i.$j.'NE' } = $node_count++;
    $hash_nodes{ $i.$j.'NW' } = $node_count++;
    $hash_nodes{ $i.$j.'SE' } = $node_count++;
    $hash_nodes{ $i.$j.'SW' } = $node_count++;
   }
 }

# create the conections
#open(my $out, ">",  "output.dot");
print "digraph G {\n";
for ($i = 0; $i <= $row; $i++)
 {
  for ($j = 0; $j <= $col; $j++)
   {
     print  $hash_nodes{ $i.$j.'N' }." -> ".$hash_nodes{ $i.$j }."\n";
     print  $hash_nodes{ $i.$j.'S' }." -> ".$hash_nodes{ $i.$j }."\n";
     print  $hash_nodes{ $i.$j.'W' }." -> ".$hash_nodes{ $i.$j }."\n";
     print  $hash_nodes{ $i.$j.'E' }." -> ".$hash_nodes{ $i.$j }."\n";
     print  $hash_nodes{ $i.$j.'NW' }." -> ".$hash_nodes{ $i.$j }."\n";
     print  $hash_nodes{ $i.$j.'NE' }." -> ".$hash_nodes{ $i.$j }."\n";
     print  $hash_nodes{ $i.$j.'SW' }." -> ".$hash_nodes{ $i.$j }."\n";
     print  $hash_nodes{ $i.$j.'SE' }." -> ".$hash_nodes{ $i.$j }."\n";

     if ($j != 0)
      {
       $cl = $j;
       $cl--;
       print  $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $i.$cl.'E' }."\n";
       print  $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $i.$cl.'NE' }."\n";
       print  $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $i.$cl.'SE' }."\n";
      }
     if ($j != $col)
      {
       $cl = $j;
       $cl++;
       print  $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $i.$cl.'W' }."\n";
       print  $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $i.$cl.'NW' }."\n";
       print  $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $i.$cl.'SW' }."\n";
      }
     if ($i != $row)
      {
       $rw = $i;
       $rw++;
       print  $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $rw.$j.'N' }."\n";
       print  $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $rw.$j.'NE' }."\n";
       print  $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $rw.$j.'NW' }."\n";
       if ($j != $col) {$cl=$j;$cl++;print  $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $rw.$cl.'NW' }."\n";}
       if ($j != 0) {$cl=$j;$cl--;print     $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $rw.$cl.'NE' }."\n";}
      }
     if ($i != 0)
      {
       $rw = $i;
       $rw--;
       print $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $rw.$j.'S' }."\n";
       print $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $rw.$j.'SE' }."\n";
       print $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $rw.$j.'SW' }."\n";
       if ($j != $col) {$cl=$j;$cl++;print $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $rw.$cl.'SW' }."\n"; ;}
       if ($j != 0) {$cl=$j;$cl--;print $hash_nodes{ $i.$j }." -> ".$hash_nodes{ $rw.$cl.'SE' }."\n"; ;}
      }

   }
 }

# putting node names as labels
for ($i = 0; $i <= $row; $i++)
 {
  for ($j = 0; $j <= $col; $j++)
   {
    printf $hash_nodes{ $i.$j }." [label=\"".$hash_nodes{ $i.$j }."-M\"]"."\n";
    printf $hash_nodes{ $i.$j.'N' }." [label=\"".$hash_nodes{ $i.$j }."-N\"]"."\n";
    printf $hash_nodes{ $i.$j.'S' }." [label=\"".$hash_nodes{ $i.$j }."-S\"]"."\n";
    printf $hash_nodes{ $i.$j.'W' }." [label=\"".$hash_nodes{ $i.$j }."-W\"]"."\n";
    printf $hash_nodes{ $i.$j.'E' }." [label=\"".$hash_nodes{ $i.$j }."-E\"]"."\n";
    printf $hash_nodes{ $i.$j.'NE' }." [label=\"".$hash_nodes{ $i.$j }."-NE\"]"."\n";
    printf $hash_nodes{ $i.$j.'NW' }." [label=\"".$hash_nodes{ $i.$j }."-NW\"]"."\n";
    printf $hash_nodes{ $i.$j.'SE' }." [label=\"".$hash_nodes{ $i.$j }."-SE\"]"."\n";
    printf $hash_nodes{ $i.$j.'SW' }." [label=\"".$hash_nodes{ $i.$j }."-SW\"]"."\n";
   }
 }

 print "}\n";

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

# Usage: matrix.pl [rows] [colums]
#
# [rows] is the rows of the  big matrix.
# [colums] is the number of colums of the big matrix.
#
# Make a graph corresponding of the Matrix regions connections. Take
# into consideration that every (i,j) is a submatrix and it has nine
# regions called: N, S, W, E, NW, NE, SE, SW and the central point.

use English;
use strict;

if ($#ARGV < 1)
 {
   print "Usage matrix.pl [rows] [colums]\n";
   exit;
 }

my $node_count = 0;

my $row = $ARGV[1] - 1;
my $col = $ARGV[0] - 1;

my %hash_nodes;

sub nodeName($$) {
    my ($i, $j) = @_;
    return "$i-$j-";
}

# creating the nodes identificator
for (my $i = 0; $i <= $row; $i++)
 {
  for (my $j = 0; $j <= $col; $j++)
   {
    $hash_nodes{ nodeName($i, $j) } = $node_count++;
    $hash_nodes{ nodeName($i, $j).'N' } = $node_count++;
    $hash_nodes{ nodeName($i, $j).'S' } = $node_count++;
    $hash_nodes{ nodeName($i, $j).'W' } = $node_count++;
    $hash_nodes{ nodeName($i, $j).'E' } = $node_count++;
    $hash_nodes{ nodeName($i, $j).'NE' } = $node_count++;
    $hash_nodes{ nodeName($i, $j).'NW' } = $node_count++;
    $hash_nodes{ nodeName($i, $j).'SE' } = $node_count++;
    $hash_nodes{ nodeName($i, $j).'SW' } = $node_count++;
   }
 }

# create the conections
my $cl;
my $rw;

print "digraph G {\n";
for (my $i = 0; $i <= $row; $i++)
 {
  for (my $j = 0; $j <= $col; $j++)
   {
     print  $hash_nodes{ nodeName($i, $j).'N' }." -> ".$hash_nodes{ nodeName($i, $j) }."\n";
     print  $hash_nodes{ nodeName($i, $j).'S' }." -> ".$hash_nodes{ nodeName($i, $j) }."\n";
     print  $hash_nodes{ nodeName($i, $j).'W' }." -> ".$hash_nodes{ nodeName($i, $j) }."\n";
     print  $hash_nodes{ nodeName($i, $j).'E' }." -> ".$hash_nodes{ nodeName($i, $j) }."\n";
     print  $hash_nodes{ nodeName($i, $j).'NW' }." -> ".$hash_nodes{ nodeName($i, $j) }."\n";
     print  $hash_nodes{ nodeName($i, $j).'NE' }." -> ".$hash_nodes{ nodeName($i, $j) }."\n";
     print  $hash_nodes{ nodeName($i, $j).'SW' }." -> ".$hash_nodes{ nodeName($i, $j) }."\n";
     print  $hash_nodes{ nodeName($i, $j).'SE' }." -> ".$hash_nodes{ nodeName($i, $j) }."\n";

     if ($j != 0)
      {
       $cl = $j - 1;
       print  $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($i, $cl).'E' }."\n";
       # checking if I have neighbor
       if ($i != 0) 
        {
         print  $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($i, $cl).'NE' }."\n";
        }
       if ($i != $row) 
        { 
         print  $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($i, $cl).'SE' }."\n";
        }
      }
     if ($j != $col)
      {
       $cl = $j + 1;
       print  $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($i, $cl).'W' }."\n";
       # checking if I have neighbor
       if ($i != 0) 
        {
         print  $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($i, $cl).'NW' }."\n";
        }
       if ( $i != $row) 
        {
         print  $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($i, $cl).'SW' }."\n";
        }
      }
     if ($i != $row)
      {
       $rw = $i + 1;
       print  $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($rw, $j).'N' }."\n";
       if ($j != $col)
        {
         print $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($rw, $j).'NE' }."\n";
        }
       if ($j != 0)
        {
         print $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($rw, $j).'NW' }."\n";
        }
       #if ($j != $col) {$cl=$j;$cl++;print  $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($rw, $cl).'NW' }."\n";}
       #if ($j != 0) {$cl=$j;$cl--;print     $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($rw, $cl).'NE' }."\n";}
      }
     if ($i != 0)
      {
       $rw = $i - 1;
       # checking if I have neighbor
       print $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($rw, $j).'S' }."\n";
       if ($j != $col)
        {
         print $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($rw, $j).'SE' }."\n";
        }
       if ($j != 0)
        {
         print $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($rw, $j).'SW' }."\n";
        }
       #if ($j != $col) {$cl=$j;$cl++;print $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($rw, $cl).'SW' }."\n"; ;}
       #if ($j != 0) {$cl=$j;$cl--;print $hash_nodes{ nodeName($i, $j) }." -> ".$hash_nodes{ nodeName($rw, $cl).'SE' }."\n"; ;}
      }

   }
 }

# putting node names as labels
for (my $i = 0; $i <= $row; $i++)
 {
  for (my $j = 0; $j <= $col; $j++)
   {
    printf $hash_nodes{ nodeName($i, $j) }." [label=\"".$hash_nodes{ nodeName($i, $j) }."-M\"]"."\n";
    printf $hash_nodes{ nodeName($i, $j).'N' }." [label=\"".$hash_nodes{ nodeName($i, $j) }."-N\"]"."\n";
    printf $hash_nodes{ nodeName($i, $j).'S' }." [label=\"".$hash_nodes{ nodeName($i, $j) }."-S\"]"."\n";
    printf $hash_nodes{ nodeName($i, $j).'W' }." [label=\"".$hash_nodes{ nodeName($i, $j) }."-W\"]"."\n";
    printf $hash_nodes{ nodeName($i, $j).'E' }." [label=\"".$hash_nodes{ nodeName($i, $j) }."-E\"]"."\n";
    printf $hash_nodes{ nodeName($i, $j).'NE' }." [label=\"".$hash_nodes{ nodeName($i, $j) }."-NE\"]"."\n";
    printf $hash_nodes{ nodeName($i, $j).'NW' }." [label=\"".$hash_nodes{ nodeName($i, $j) }."-NW\"]"."\n";
    printf $hash_nodes{ nodeName($i, $j).'SE' }." [label=\"".$hash_nodes{ nodeName($i, $j) }."-SE\"]"."\n";
    printf $hash_nodes{ nodeName($i, $j).'SW' }." [label=\"".$hash_nodes{ nodeName($i, $j) }."-SW\"]"."\n";
   }
 }

 print "}\n";

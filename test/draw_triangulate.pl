#!/usr/bin/perl

use strict;
use feature qw/say/;
use Cairo;

my $file_in = shift;
my $file_out = shift;

my $x_min;
my $x_max;
my $y_min;
my $y_max;

my @data;

open IN,'<',$file_in or die "failed to open input file $file_in: $!";
while (<IN>)
{
    chomp;
    my ($x, $y) = split /\t/;
    $x_min = $x if !defined $x_min or $x_min > $x;
    $x_max = $x if !defined $x_max or $x_max < $x;
    $y_min = $y if !defined $y_min or $y_min > $y;
    $y_max = $y if !defined $y_max or $y_max < $y;

    push @data, [$x, $y];
}
close IN;

say "x: $x_min - $x_max, y: $y_min - $y_max";

my $width = $x_max - $x_min;
my $height = $y_max - $y_min;

say "size: $width x $height";

my $num_tri = @data / 3;

my $surface = Cairo::SvgSurface->create($file_out, $width, $height);
my $context = Cairo::Context->create($surface);

$context->translate(-$x_min, $height + $y_min);

$context->set_line_width(($width+$height)/150);
$context->set_line_join('round');
$context->set_line_cap('round');

my $i_tri;
while (@data > 0)
{
    my $hue = $i_tri / $num_tri;
    my ($r, $g, $b) = hsv2rgb($hue, 1, 1);

    my $p1 = shift @data;
    my $p2 = shift @data;
    my $p3 = shift @data;
    
    $context->move_to($p1->[0], -$p1->[1]);
    $context->line_to($p2->[0], -$p2->[1]);
    $context->line_to($p3->[0], -$p3->[1]);
    
    $context->set_source_rgba($r, $g, $b, 0.2);
    $context->fill_preserve();
    $context->set_source_rgba($r, $g, $b, 0.6);
    $context->stroke();
    
    $i_tri++;
}


sub hsv2rgb
{
    my ($h, $s, $v) = @_;
    my $h2 = $h * 6;
    
    my $phase = $h2;
    while ($phase > 2) {$phase -= 2}
    
    my $c = $s * $v;
    my $x = $c * (1 - abs($phase - 1));
    
    if ($h2 < 1) { return ($c, $x, 0) }
    elsif ($h2 < 2) { return ($x, $c, 0) }
    elsif ($h2 < 3) { return (0, $c, $x) }
    elsif ($h2 < 4) { return (0, $x, $c) }
    elsif ($h2 < 5) { return ($x, 0, $c) }
    else            { return ($c, 0, $x) }
}
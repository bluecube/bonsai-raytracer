use strict;
use warnings;

package Raw;

# load a file with raw faces.
# returns hash ref with array refs "vertices",
# and "polygons"
sub load{
	my $file = shift;

	my @vertices = ();
	my @fragments = ();

	local $/ = "\n";

	open IN, "<", $file or return "Error opening file $file";

	my %hash;

	while(<IN>){
		my @v = split;

		return "Wrong file format" unless(@v == 9 || @v == 12);

		my @indices;

		for(my $i = 0; $i < @v; $i += 3){
			my $id = $hash{$v[$i]}{$v[$i + 1]}{$v[$i + 2]};

			if(!defined($id)){
				my @arr = @v[$i .. $i + 2];
				push @vertices, \@arr;
				$id = $#vertices;
				$hash{$v[$i]}{$v[$i + 1]}{$v[$i + 2]} = $id;
			}

			push @indices, $id;

		}

		push @fragments, \@indices;
	}

	close IN;

	{ vertices => \@vertices, polygons => \@fragments };
}

1;

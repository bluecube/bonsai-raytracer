use strict;
use warnings;

package Scene;

use Regexp::Grammars;
use Data::Dumper;

my %functions = (
	'T'		=> '',
	'inv'		=> '',

	'sin'		=> '',
	'cos'		=> '',
	'tan'		=> '',
	'asin'		=> '',
	'acos'		=> '',
	'atan'		=> '',
	'atan2'		=> '',

	'log'		=> '',
	'exp'		=> '',

	'sign'		=> '',
	'ceil'		=> '',
	'floor'		=> '',
	'round'		=> '',

	'abs'		=> '',

	'hsv'		=> '',

	'sphere'	=> '',
	'cube'		=> '',
	'cylinder'	=> '',
	'plane'		=> '',
	'polygonal'	=> '',
	'CSG_and'	=> '',
	'CSG_or'	=> '',

	'rotation'	=> '',
	'translation'	=> '',
	'shearing'	=> '',
	'scaling'	=> '',

	'inside'	=> '',
	'trace'		=> '',
);

my %constants = (
	'PI'		=> number(3.1415926535897932384626),
	'true'		=> number(1),
	'false'		=> number(0),

	'time'		=> number(0),
	'filename'	=> string(''),
	'renderer'	=> string(''),
);

my %variables = ();

my $parser = qr{
#	<debug: on>
	<BOF> <MATCH=Commands> <EOF>

	<rule: Commands>
		(?: <[Command]> <.Semicolon> )*
	
	<rule: Command>
		(?: var <Name> ) |
		(?: var <Name> = <Expression> ) |
		(?: <LValue> = <Expression> ) |
		<Expression> |
#		(?: <LValue> = \{ <MATCH=Commands> \} ) |
		<.ws>
	
	<rule: LValue>
		<Var> |
		<Var> <[Subscript]>*

	<rule: Var>
		<%variables> |
		<Name>
	
	<rule: Constant>
		<%constants>
	
	<rule: Expression>
		<String> |
		<NumericExpression>
	
#	<rule NumericExpression>
#		(?: <Plusminus> <Term> ) |
#		(?: <Expression> <AdditiveOp> <Term> ) |
#		<Term>

	<rule: NumericExpression>
		(?: <Plusminus> <[Term]> ** <[AdditiveOp]> ) |
		(?: <[Term]> ** <[AdditiveOp]> )

#	<rule: Term>
#		(?: <Term>  <MulOp> <Factor> ) |
#		<Factor>
	
	<rule: Term>
		<[Factor]> ** <[MulOp]>
	
	<rule: Factor>
		<Number> |
		<PossiblyMatrix> <[Subscript]>*
	
	<rule: PossiblyMatrix>
		<Var> |
		<Constant> |
		(?: \( <MATCH=Expression> \) ) |
		<Function> |
		<Matrix>

	<rule: Function>
		<%functions> \( <[Expression]> ** <.Comma> \)
	
	<rule: Subscript>
		\[ <[MATCH=Expression]> \]
	
	<rule: Matrix>
		\< <[MatrixRow]> ** <.Semicolon> \>
	
	<rule: MatrixRow>
		<[Expression]> ** <.Comma>

	<token: Semicolon>
		;
		
	<token: Comma>
		,
	
	<token: Name>
		[a-zA-Z_][a-zA-Z_0-9]*

	<token: Plusminus>
		[+-]
	
	<token: AdditiveOp>
		(?: \+ | - | \| )
	
	<token: MulOp>
		(?: \* | / | % | & )
	
	<token: String>
		"(?:[^"]*(?:\\")?)*"
	
	<token: Number>
		[0-9]+ #(?:.[0-9]+)?(?:[eE]<Plusminus>?[0-9]+)?\b
	
	<token: ws>
		(?: \s | \#[^\n]*$ )*
	
	<token: BOF>
		\A
	
	<token: EOF>
		\z |
		__EOF__
}xms;

sub number{
	{type => 'matrix', rows => 1, cols => 1, value => {1 => {1 => shift}}};
}

sub string{
	{type => 'string', value => shift};
}

sub load{
	my $filename = shift;
	my $renderer = shift;

	local $/;
	undef $/;

	open(my $fh, '<', $filename) or die $!;

	$constants{'filename'} = string($filename);
	$constants{'renderer'} = string($renderer);

	my $file = <$fh>;

	print "Matches\n" if $file =~ $parser;

	close $fh;

	print Dumper(\%/);
exit;


	{aspect => 16/9};
}

1;

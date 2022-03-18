#!/usr/bin/perl

#
# expand.pl - expand postscript macros in abctabfonts
#



# global variable intitializations
#---------------------------------------------------

$INFILE = "";

$PGM = "expand.pl";
$USAGEMSG = "USAGE: " . $PGM . " <infile>\n";

$PENLINETO = "% penlineto start\n" .
	"currentpoint 4 2 roll lineto\n" .
	"currentpoint 4 2 roll -2 -2 rlineto\n" .
	"2 1 roll -2 add 2 1 roll -2 add lineto\n" .
	"2 2 rlineto eofill moveto % penliento end\n";

$PENCURVETO = "% pencurveto start\n" .
	"6 copy currentpoint 8 2 roll curveto\n" .
	"currentpoint 10 2 roll -2 -2 rlineto\n" .
	"4 2 roll pop pop 4 2 roll 6 2 roll\n" .
	"3 { 6 5 roll -2 add 6 5 roll -2 add } repeat\n" .
	"curveto 2 2 rlineto eofill moveto % pencurveto end\n";

# main program
#---------------------------------------------------

#
# parse command line
#
while (@ARGV) {
    my $opt = shift @ARGV;
    if ($opt eq "-?") {     # help message
        print $USAGEMSG;
        exit 0;
    } 
    elsif ($opt =~ /^-/) {  # illegal option
        print STDERR $USAGEMSG;
        exit 1;
    }
    else {                  # input file
        $INFILE = $opt;
        if (! -e $INFILE) {
            print STDERR "Cannot find $INFILE\n";
            exit 1;            
        }
    }
}

#
# process postscript input
#
if ($INFILE) {
    open(IFH, $INFILE) or die("Cannot open ", $INFILE, "\n");
} else {
    print STDERR $USAGEMSG;
	exit 1;
}
while (<IFH>) {

	# definitions to ignore
    if (m+^/penend+) {
        next;
    }
    if (/Francisquepen/) {
        next;
    }
    if (m+^/penlineto+ || m+^/pencurveto+) {
		$line = "";
		while ($line !~ / bind def/) {
		  $line = <IFH>;
		}
        next;
	}
	
    # expand macros
	s/penlineto/$PENLINETO/g;
	s/pencurveto/$PENCURVETO/g;


	# print other lines unchanged
	print;

}
close(IFH);


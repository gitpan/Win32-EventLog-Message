#//  compile_message.pl
#//  Perl script to compile the Win32::EventLog::Message 
#//  message table.
#//  (c) Dave Roth
#//  Courtesy of Roth Consulting
#//  http://www.roth.net/
#//
#//  Available under the GNU license.
#//  2000.03.19

$MessageSource  = "PerlMsg.mc";
$TemplateFile   = "rc_template.txt";
$ResourceFile   = "PerlMsg.rc";

print "Compiling message resource then merging with the resource template...\n\n";

print "Deleting existing binary message files (*.bin).\n";
`del *.bin`;

print "Compiling the message table.\n";
`mc -v -w $MessageSource`;

print "Reading the resource template file.\n";
open( TEMPLATE, "< $TemplateFile" ) || die "Could not open template '$TemplateFile': $!\n";
$TemplateData = join( "", <TEMPLATE> );
close( TEMPLATE );

print "Reading the newly created resource file.\n";
open( FILE, "< $ResourceFile" ) || die "Could not open resource file '$ResourceFile': $!\n";
$ResourceData = join( "", <FILE> );
close( FILE );

print "Opening the resource file for writing.\n";
open( RESOURCE, "> $ResourceFile" ) || die "Could not save the resource file '$ResourceFile': $!\n";

print "Merging the template and fresh resource file.\n";
print RESOURCE $TemplateData, "\n\n";
print RESOURCE $ResourceData, "\n\n";

close( RESOURCE );

print "Finished.\n";

#  Message.pm
#  The Win32::EventLog::Message Perl Module File.
#  (c) Dave Roth
#  Courtesy of Roth Consulting
#  http://www.roth.net/
#
#  2000.06.12

package Win32::EventLog::Message;

$PACKAGE = $Package = "Win32::EventLog::Message";

$VERSION = 20000624;
require Exporter;
require DynaLoader;
use Win32::EventLog;

@ISA= qw( Exporter DynaLoader Win32::EventLog );

    # Items to export into callers namespace by default. Note: do not export
    # names by default without a very good reason. Use EXPORT_OK instead.
    # Do not simply export all your public functions/methods/constants.
@EXPORT = qw(

    EVENT_ID

    EVENT_SUCCESS_1
    EVENT_SUCCESS_2
    EVENT_SUCCESS_3

    EVENT_INFO_1
    EVENT_INFO_2
    EVENT_INFO_3

    EVENT_WARNING_1
    EVENT_WARNING_2
    EVENT_WARNING_3

    EVENT_ERROR_1
    EVENT_ERROR_2
    EVENT_ERROR_3

    EVENT_SOURCE

    RegisterSource
    UnRegisterSource
    GetSource
    GetSourceList
    GetEventLogList
);


@EXPORT_OK = qw(
);	

bootstrap $Package;

sub AUTOLOAD 
{
    # This AUTOLOAD is used to 'autoload' constants from the constant()
    # XS function.  If a constant is not found then control is passed
    # to the AUTOLOAD in AutoLoader.

    my( $Constant ) = $AUTOLOAD;
    my( $Result, $Value );
    $Constant =~ s/.*:://;

    $Result = Constant( $Constant, $Value );

    if( 0 == $Result )
    {
        # The extension could not resolve the constant...
        $AutoLoader::AUTOLOAD = $AUTOLOAD;
	    goto &AutoLoader::AUTOLOAD;
        return;
    }
    elsif( 1 == $Result )
    {
        # $Result == 1 if the constant is valid but not defined
        # that is, the extension knows that the constant exists but for
        # some wild reason it was not compiled with it.
        $pack = 0; 
        ($pack,$file,$line) = caller;
        print "Your vendor has not defined $Package macro $constname, used in $file at line $line.";
    }
    elsif( 2 == $Result )
    {
        # If $Result == 2 then we have a string value
        $Value = "'$Value'";
    }
        # If $Result == 3 then we have a numeric value

    eval "sub $AUTOLOAD { return( $Value ); }";
    goto &$AUTOLOAD;
}

1;

__END__

=head1 NAME

Win32::EventLog::Message - Extension enabling Win32 Perl Event Logs to embed messages.


=head1 SYNOPSIS

    use Win32::EventLog;
    use Win32::EventLog::Message;
    Win32::EventLog::Message::RegisterSource( 'System', 'My Perl Message Source' );

    $Event = Win32::EventLog->new( 'My Perl Message Source' ) || die;

    $Event->Report( 
        {
            EventID     => EVENT_ID,
            Strings     => "Everything is okay.\nReally, it's okay.",
            EventType   => EVENTLOG_SUCCESS_TYPE,
        }
    );


=head1 DESCRIPTION

This extension provides a message table for Win32 Event Log entries.
With this extension event log source names can be created and used.

=head1 FUNCTIONS

C<RegisterSource()>

C<UnRegisterSource()>

C<GetSource()>

C<GetSourceList()>

C<GetEventLogList()>

=over 4


=item GetEventLogList( [$Machine, ] \@List )

This will populate the specified array with the the list of Event Logs that are available on
the specified machine.  If no machine is specified the local machine is assumed.

B<Example:>

    GetEventLogList( "\\\server", \@List );
    foreach my $EventLogName ( sort( @List ) )
    {
        print "$EventLogName\n";
    }

B<Result:>
Number of Event Logs that are available on the specified machine.

=item GetSource( $EventLog, $SourceName, \%Config )

This will populate the specified hash with the configuration for the specified
registered source name in the specified event log.

B<Example:>

    GetSourceList( 'System', \@List );
    foreach $Name ( sort( @List ) )
    {
        if( GetSource( 'System', $Name, \%Config ) )
        {
            print "$Name message file: $Config{path}\n";
        }
    }

B<Result:>
TRUE if successful (and populates the specified hash) otherwise returns FALSE.

=item GetSourceList( $EventLog, \@List )

This will populate the specified array with a list of source names registered under 
the specified event log.

B<Example:>

    # This will generate a list of all source names registered
    # under the System log
    $Total = GetSourceList( 'System', \@List );

B<Result:>
Total number of registered sources.

=item RegisterSource( $EventLog [, $SourceName ] )

This registers a new EventLog source name.  

B<Example:>

    # This will create a new source name under the System log called
    # "My Perl Source"
    RegisterSource( 'System', 'My Perl Source' );

B<Returns:>
C<TRUE> if successful otherwise C<FALSE>.

=item UnRegisterSource( $EventLog, $SourceName )

This will unregister an event log source name. This will remove the event message source
from the Event Log configuration.  Any entry in the Event Log that refers to the specified
source name will result in an entry log description of something similar to:

    The description for Event ID ( 0 ) in Source ( My Perl Log ) cannot 
    be found. The local computer may not have the necessary registry 
    information or message DLL files to display messages from a remote 
    computer.

B<Example:>

    # This will remove a source name under the System log called
    # "My Perl Source"
    UnRegisterSource( 'System', 'My Perl Source' );

B<Result:>
C<TRUE> if successful otherwise C<FALSE>.

=cut

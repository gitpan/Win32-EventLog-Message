use Win32::EventLog;
use Win32::EventLog::Message;

$PerlSource = "My Perl Log";

Win32::EventLog::Message::RegisterSource( 'System', $PerlSource );

$Event = Win32::EventLog->new( $PerlSource ) || die;

$Result = $Event->Report( 
    {
        EventID     => EVENT_ID,
        Strings     => "Everything is okay.\nReally, it's okay.",
        EventType   => EVENTLOG_SUCCESS_TYPE,
    }
);

print "Report result: $Result\n";

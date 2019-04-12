#Process involved before reaching the final code

1) Started by testing the sample code provided in the link.
2) There were two sample codes, one for recording the data and one for playing.
3) The sample code that recorded the data, wrote it to STDOUT. So,I first tred to store in a file.
4) The second sample code(the one used for playback) read the data from the same file and tried to play it back. But it was giving error of "bad file descriptor".
5) Then I created a client-server model(using TCP/IP socket), where the data being recorded was being written to the data buffer sent to the server.
6) The server could now play the audio properly. But there were issues with the voice quality-
    echo
    noise
    volume levels were not stable, some words would be heard clearly, while others sounded like a whisper.

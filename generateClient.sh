(printf "PASS $1\nNICK $2\nUSER u s e r\nJOIN #chan1\n"; cat) | nc localhost 9999

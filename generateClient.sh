(printf "PASS $1\r\nNICK $2\r\nUSER u s e :r v\r\nJOIN #chan1\r\n"; cat) | nc -C localhost 9999

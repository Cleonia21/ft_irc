# SUMMARY

Simple IRC Server based on the **[RFC](https://datatracker.ietf.org/doc/html/rfc2812)** written in C++.

ft_irs does not support server-server communication and is best used with **[KVirc](http://www.kvirc.net)** (or any client that send packets terminated with `\r\n`).

# Installation

Copy paste the following code on your terminal:

```shell
cd ~ && cd Downloads && git clone https://github.com/Cleonia21/ft_irc.git && cd ft_irc && make
```

# Run

Start server:

```shell
./ircserv <port> <password>
```

Without using a graphical client, you can connect to the server like this:

```shell
nc localhost <port>
```
Then
```shell
PASS <password>
NICK <nickname>
USER <username> <hostname> <servername> <realname>
```

![Image of ft_irc](https://github.com/Cleonia21/ft_irc/blob/master/example.png)

## Handled commands

This following list of commands are handled on our server, since most of them were not asked by the subject there is certain masks and parameters we may have omitted for your convenience:

```
 - PASS
 - NICK
 - USER
 - MODE
 - JOIN
 - KICK
 - PART
 - OPER
 - KILL
 - LIST
 - QUIT
 - TOPIC
 - NAMES
 - NOTICE
 - PRIVMSG
 - INVITE
 - PING
```

## Handled modes :

The following list of modes are handled by the server:

```
USER MODES :
i - marks a users as invisible;
w - user receives wallops;
o - operator flag;
s - marks a user for receipt of server notices.

CHANNEL MODES :
for users :
    - o : channel operator
    - v : voice
for channels :
    - i : invite only
    - m : moderated
    - n : outside privmsg disabled
    - p : private
    - t : topic locked
    - k : key locked
    - l : user limit
```

# DOCS

*we recommend following the 281x versions instead as they not experimental*
- [RFC1459](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC2810](https://datatracker.ietf.org/doc/html/rfc2810)
- [RFC2811](https://datatracker.ietf.org/doc/html/rfc2811)
- [RFC2812](https://datatracker.ietf.org/doc/html/rfc2812)
- [RFC2813](https://datatracker.ietf.org/doc/html/rfc2813)

## MADE WITH LOVE BY :

- [Cleonia](https://github.com/Cleonia21/) aka **Daniil Kharitonov**
- [Sfaycer](https://github.com/Sfaycer/)   aka **Aleksandr Evseev**
- [mshbbrv](https://github.com/mshbbrv/)   aka **Misha Bobrov**

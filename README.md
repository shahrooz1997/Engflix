# Engflix
This is a great application that helps you learn English in the most efficient way, through movies.
This breaks down a movie to the parts based on the subtitle, and pause after showing each part, so you can practice by speaking what you heard.

## Compile
This is written specifically for Ubuntu OS. You also need to install VLC Media Player and dbus.
```sh
sudo apt-get install vlc dbus git cmake libxmu-dev libglib2.0-dev libdbus-1-dev
```
Then run the following commands:
```sh
git clone https://github.com/shahrooz1997/Engflix.git
cd Engflix
mkdir mybuild
cd mybuild
cmake ../
make -j4
```

## Execution
First run VLC playing the movie you would like to watch, and add its subtitle to the player. Then, execute this program in terminal passing the the address to the subtitle as its argument.
```sh
Usage: ./English_Teacher [subtitle path]
```
To make using of the application more comfortable, right click on the terminal running the application, and check "Always on Top". Then, you may resize the terminal to the smallest possible and put it on top right. Start learning English using the following control keys. (:

Make sure that focus of your keyboard is on the terminal.

## Controls

w: play/pause

a: move to the previous subtitle

s: repeat the current subtitle

d: continue playing the movie

f: fast-forward to the next subtitle (Please note that pressing f will seek the player to the start of the next subtitle while pressing d will not.)

q: sync the application if you seek the player manually

## Todo

* Adding recording of your voice after it shows you a subtitle

* Play back your recorded voice on the silent movie (I think of maybe changing the paly rate to match it with your speed of speaking)

* Adding bookmart feature so you can save your favorite idioms

* Using Leitner system to help you remember the bookmarked idioms

* Adding Google API to access its dictionary for hard words

* Use window controller to setup a workspace for this app

## Contact

Hamidreza Zare - [Homepage](http://shahrooz1997.github.io/) - hkz5146@psu.edu

Project Link: [https://github.com/shahrooz1997/Engflix](https://github.com/shahrooz1997/Engflix)

I hope you find this application useful. Your opinions/contributions are highly appreciated.

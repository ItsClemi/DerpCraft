# DerpCraft

DerpCraft is a Minecraft Server Emulator for the client version: 1.5.1 which i wrote with a friend back in 2013.
It contains the most basic stuff like: packet handling, world creation, user manager, etc...

But the heart of the server is an algorithm i called 'Minecraft Chunk Chain Storage'.
It's just an 2d array of chunks which go in every axis(x y /x -y / -x -y / -x y ) direction.

<p align="center">
  <img src="https://github.com/ItsClemi/DerpCraft/blob/master/Storage.png?raw=true" alt="Storage"/>
</p>



The bad part of this algorithm is that it needs a lot of memory. 
But on the other hand its very fast to convert x/y coords to a chunk ptr.

This project requires:
* TBB
* zLib
* Crypto++

# EugenChess
A library that seeks to provide abstractions for chess engines, as well as engine - GUI communication.

Chess engines are usually standalone applications with no GUI to speak of. Since GUI is a needed comodity when dealing with engines, the engines are usually run in conjunction and in cooperation with chess GUIs. The two processes communicate through interprocess communication, following some kind of protocol, of which the most popular is UCI.
To the best of author's knowledge, the engines, more often than not, fail (or do not attempt) to decouple the engine logic from the communication protocol they support. There is room for improvement in code organisation in chess engines, and this library seeks to facilitate that.
It will provide an abstraction for the communication protocol and an abstraction for the engine functionality. This is WIP currently.
Every interface has its limitations in terms of how many diverse protocols it can cover, however this library seeks to provide reasonably general utility. It can be extended if need be.

# Created an AI in C to play the game 'Fury Of Dracula'
How it works:
1. the Game Engine invokes the AI, giving it an appropriate View of the current Game State.
2. AI decides on a move based on traversing a pre-implemented graph of the game map.
2. takes the last move suggested by the AI. The Game Engine then takes the move, records it, updates its own game state, and then move on to the next AI. The interface for your AIs is very simple: they take a View as input, and send a new location and a message as their output.

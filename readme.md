# Battlefield (the strategy game)

## Installation

Install qt 5.5. Then compile the bat3.pro file using qmake, perhaps by opening QT creator on bat3.pro and hitting run.

## Game

Turn based strategy game. Build buildings which produce units, attachments to units, and make money. Buy units and attachments, and attack the other player. Units block both movement and attack vectors, complicating tactics. You win by eliminating the entirety of the other player.

## History

This started out as a board game I created when I was 8 years old. But I was frustrated by the limitations of the medium. When I learned how to code, I immediately started working on the game. I basically learned how to code making this game.

The art started by pulling clip-art from the internet, and then I have been slowly replacing that by my own art.

After getting basic functionality down, I immediately started working on the AI, and after rewriting it 3 times, it is still not too great.

Having the Debug_Macro_Moves #defined in globalinfo.h allows you to veiw the analysis that the AI is using to make decisions.

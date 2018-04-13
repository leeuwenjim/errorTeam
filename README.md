Dit is de GitHub repository voor ons project van Technische Informatica. 
Ons zelfrijdend voertuig heeft de naam Arnold gekregen, we hebben deze naam in de code gebruikt voor gemak en stijl.

In deze readme staan de requirements en kort wat we gebruikt hebben.

Requirements:
 - Firmware 1.4.4 voor de BrickPi

Gebruikte onderdelen:
 - 31313 Mindstorms EV3
 - BrickPi met Raspberry Pi 3 model B
 - Accupakket
  
Gebruikte stijlen:
- Doxygen commenting
  - /// Voor main comments
  - /** Voor gedetaileerde uitleg */ 
- camelCase voor functies, uitgezonderd exit signal handler ivm opvallendheid

Clone repository:
- git clone https://github.com/leeuwenjim/errorTeam.git
- cd errorTeam/

Compile & Run lineFollower:
 - g++ BrickPi3.cpp Arnold.cpp lineFollower.cpp -std=c++14 -o lineFollower
 - ./lineFollower


Compile & Run gridFollower:
- g++ BrickPi3.cpp Arnold.cpp aStarSourceFile.cpp gridFollowProgram.cpp -std=c++14 -o gridFollower
 - ./gridFollower


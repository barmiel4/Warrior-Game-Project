# Warrior Game Project

## Core Idea
This project is a RPG game inspired by _God of War_ and the like, featuring Axe attacks, blocking and parrying with 2 types of enemies attacking the player. Project utilizes Gameplay Ability System for any gameplay abilities for both player and enemies and uses Lyra Sample Project architecture as a reference to provide a modular and flexible framework.

## Implemented features
### Weapons
By pressing the **1** key, player can equip their weapon. Once weapon is equipped, apart from attaching the weapon to the character's hand, player is granted all combat abilities associated with the weapon, such as Attack, Block and Unequip. Also by switching the input mapping context and overriding the default input, weapon can be uneqipped by pressing **1** again, which takes away all the abilities metioned above.

### Attacks
Player light and heavy attacks, that are also implemented inside Gameplay Ability System, are granted when given weapon is equipped. Each of them has a combo counter that influences damage dealt to the enemy on impact. Also a special combo move - 3 light attack + 1 heavy attack - can be performed. When player performs 3 light attack and clicks the Heavy attack button a gameplay event is sent. Heavy Attack ability recives this event and performs the heavy attack that deals the most damage with the appropriate animation.

### Health and Dealing Damage
All game enitites attributes such as health, rage, attack power and defense level are stored inside a custom attribute set. To set up the base values for these attributes, gameplay effects are applied to both the hero character and the enemies. One gameplay effect is for setting up the max values and the other allows to set the current ones.
To handle such events like dealing damage, another gameplay effect is used, with a custom Effect Calculation Exection that allows to use other attributes, like defense level or combo count while calculating the damage output.

### Enemy AI
Every AI actor utilizes few interesting features:
* Crowd Avoidance - project uses Detour Crowd Manager - an algorithm that allows AI to know how to avoid obstacles and other AI actors while staying on the Nav Mesh and retaining realistic behavior.
* Environment Query System - by generating a list of possible points by given rules and employing a ranking system, AI can pick the best position in the game world inside Nav Mesh and traverse to it.
* Behavior Trees - allows to create a decision graph, that each AI can evaluate and choose the most approprate reaction to the events occurring during gameplay.

There are two types of AI actors in the game, that both feature distinctive behavior:
#### Melee Enemy 
Its main purpose is to chase and surround the player, with couple of rules:
* When the player is too far, it tries to chase him
* When close to the player, AI enters strafing state using the EQS-generated points around the player to pick a strafing position
* When in strafing state, AI has a random chance to attack
* When player gets too close, AI abandons whatever task it had and immediately start attacking the player

#### Ranged Enemy 
Goal of this enemy is to reateadly fires projectiles at the player while trying the keep the greatest distance. It featutures the following behavior:
* It tries to find the best shooting position based on EQS-generated list of points
* When it reaches this position, AI tries to shoot at the player
* If player tries to get close, AI searches for another shooting position
* If player is close enough(i.e. in the melee range), AI performs an unblockable melee attack

### Custom nodes
There is a couple of custom nodes implemented in C++ that are used thourghout the project:
* **Orient Enemy Rotation To Target** Service -  calculates and rotate the enemy to the attack target
* **Execute Task On Tick** ability node - a gameplay abilty latent node that can be used like an event tick inside gameplay abilities blueprint

### Solutions from Lyra
* Input Biding - similiar to Lyra, this project uses custom input component deriving from the Enhanced Input Component, that binds both native input and gameplay abilties based on the gameplay tags associated with them.
* Linked Anim Layer - to support more flexible animation switching, project uses custom animation layers. It allows to set given layer with appropriate animations/blend spaces bases on the equipped weapon along with enabling/disabling them whether or not any weapon is equipped.
* Pawn Components - custom Pawn Extension components used for Combat(weapons management and weapon collision events handling) and for UI(sending delegate and Widget communication) 

### Tech and Tools
* Unreal Engine 5
* C++
* Enhanced Input Component
* Gameplay Ability System
* Gameplay Tags
* Attribute Set
* Behaviour Trees
* Crowd Avoidance for AI actors
* Custom Behaviour Tree Tasks, Services and Decorators
* Environment Query System

## Idea origin
Project is developed according to the tutorial **Create advanced RPG combat experience in Unreal Engine 5 through Gameplay Ability System(GAS) using C++**. Implementation is completed in 70% with the core functionalities mentioned above already implemented and tested.

# Warrior Game Project

## Core Idea
Project is a RPG game inspired by _God of War_ and the like, featuring Axe attacks, blocking and parrying with 2 types of enemies attacking the player. Project utilizes Gameplay Ability System framework for any gameplay abilities for both player and enemies and uses Lyra Sample Project architecture as a reference to provide a modular and flexible framework.

## Implemented features
### Weapons
By pressing the **1** key, player can equip their weapon. Apart from attaching the weapon to the his hand, player is granted all combat abilities associated with the weapon, such as Attack, Block and Dodge. Also by switching the mappings context and overriding the default input, weapon can be uneqipped by pressing **1** again, which takes away all the abilities metioned above.

### Attacks
Player light and heavy attacks, that are also implemented inside Gameplay Ability, are granted when given weapon is equipped. Each of them has a combo counter that influences damage dealt to the enemy on impact. Also a special combo move - 3 light attack + 1 heavy attack - can be performed. When player performs 3 light attack and clicks the Heavy attack button a gameplay event is sent. Heavy Attack ability recives this event and performs the heavy attack that deals the most damage with the appropriate animation.

### Health and Dealing Damage
attribute set + gameplay effects

### Enemy AI
...

### Solutions from Lyra
...

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
Project is developed according to the tutorial **Create advanced RPG combat experience in Unreal Engine 5 through Gameplay Ability System(GAS) using C++**. My implementation completed in 70% with the core functionalities mentioned above already implemented and tested.

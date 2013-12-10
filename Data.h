#pragma once
const static char fileStart[] = "<roblox xmlns:xmime=\"http://www.w3.org/2005/05/xmlmime\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"http://www.roblox.com/roblox.xsd\" version=\"4\"><Item class=\"Workspace\"><Item class=\"Script\"><Properties><ProtectedString name=\"Source\">\
b,d,i,B,n,V,r,I={4,1,1,14,9,0,5,17,17,15,15,2,11,11,11,11,10,1,2,0,11,16,12,2,9,0,0,0,12,0,0,0,12,0,7,0,0,0,0,0,8,6,14,14,3,15,9,13,5,0,0,0,9,9,0,11,7,9,0,1,12,12,0,0,0,0,14,0,0,0,0,0,11,11,0,0,0,0,7,7,0,3,0,9,0,0,3,9,8,17,0,0,0,0,0,0,4,12,0,0,0,0,0,0,0,0,0,3,12,1,0,3,0,3,0,0,0,6,17,7,2,5,6,8,9,9,0,2,11,5,0,0,16,9,9,9,15,7,0,0,0,0,0,0,6,9,0,0,0,0,0,15,3,6,7,7,0,12,3,0,0,10,9,9,0,0,0,0,0,2,0,3,5,7},script.Value.Value,1,string.byte,0,Vector3int16.new,function()c,i=B(d,i),i+1 if c==255 then c=0 elseif c==92 then c,i=B(d,i),i+1 if c==48 then c=255 elseif c~=92 then c=c+127 end end return c end,function()t=r()*2^24+r()*2^16+r()*256+r()return t&gt;2^31 and 2^31-t or t end while i&lt;#d do Z=I()*16 X=I()*16 if 100*i/#d&gt;n then print(n..'%')n=n+1 wait()end for x=X,X+15 do for z=Z,Z+15 do y=0 while y&lt;255 do p=b[r()]N=r()if p then workspace.Terrain:SetCells(Region3int16.new(V(-x,y,z),V(-x,y+N-1,z)),p,0,0)end y=y+N end end end end print('Done!  Please save the game.')script:Destroy()\
</ProtectedString></Properties><Item class=\"StringValue\"><Properties><string name=\"Name\">Value</string><string name=\"Value\">";
const static char fileEnd[] = "</string></Properties></Item></Item></Item></roblox>";
/*
enum robloxBlocks{
	RTB_EMPTY,
	RTB_GRASS,
	RTB_SAND,
	RTB_BRICK,
	RTB_GRANITE,
	RTB_ASPHALT,
	RTB_IRON,
	RTB_ALUMINIUM,
	RTB_GOLD,
	RTB_WOODPLANK,
	RTB_WOODLOG,
	RTB_GRAVEL,
	RTB_CINDERBLOCK,
	RTB_MOSSYSTONE,
	RTB_CEMENT,
	RTB_REDPLASTIC,
	RTB_BLUEPLASTIC,
	RTB_WATER,
	
	//PB_SAPLING,
	//PB_LEAVES,
	//PB_GLASS,
	//PB_WOOL,
	//PB_WOOD_DOOR,
	//PB_IRON_DOOR,
	//PB_ICE,
	//PB_SNOW,
	//PB_CLAY,
	//PB_PUMPKIN,
	//PB_NETHERRACK,
	//PB_SOULSAND,
	//PB_STAINEDGLASS,
	//PB_IRONBARS,
	//PB_GLASSPANE,
	//PB_MELON
};


const static char blockMappings[] = {
	0, //air
	RTB_GRANITE, //stone
	RTB_GRASS, //grass
	RTB_GRASS, //dirt
	RTB_CEMENT, //cobblestone
	RTB_WOODPLANK, //wood planks
		0, //saplings
	RTB_ASPHALT, //bedrock
	RTB_WATER, //water
	RTB_WATER, //water still
		RTB_REDPLASTIC, //lava
		RTB_REDPLASTIC, //lava still
	RTB_SAND, //sand
	RTB_GRAVEL, //gravel
		RTB_GRAVEL, //gold ore
		RTB_GRAVEL, //iron ore
		RTB_GRAVEL, //coal ore
	RTB_WOODLOG, //wood
		RTB_GRASS, //leaves
	RTB_SAND, //sponge
		0, //glass
		RTB_GRAVEL, //lapis ore
	RTB_BLUEPLASTIC, //lapis
	RTB_CINDERBLOCK, //dispenser
	RTB_SAND, //sandstone
	RTB_WOODPLANK, //note block
	0, //bed
	0, //powered rail
	0, //detector rail
	RTB_CINDERBLOCK, //sticky piston
	0, //cobweb
	0, //tall grass
	0, //dead bush
	RTB_CINDERBLOCK, //piston
	0, //piston head
	RTB_ALUMINIUM, //wool
	0, //piston move
	0, //yellow flower
	0, //red flower
	0, //brown mushroom
	0, //red mushroom
	RTB_GOLD, //gold block
	RTB_IRON, //iron block
	RTB_CEMENT, //double stone slab
	RTB_CEMENT, //stone slab
	RTB_BRICK, //bricks
	RTB_REDPLASTIC, //tnt
	RTB_WOODPLANK, //bookshelf
	RTB_MOSSYSTONE, //mossy cobblestone
	RTB_ASPHALT, //obsidian
	0, //torch
	0, //fire
	0, //spawner
	RTB_WOODPLANK, //stairs
	RTB_WOODPLANK, //chest
	0, //redstone wire
		RTB_GRAVEL, //diamond ore
	RTB_ALUMINIUM, //diamond block
	RTB_WOODPLANK, //crafting table
	0, //wheat
	RTB_GRASS, //farmland
	RTB_CINDERBLOCK, //furnace
	RTB_CINDERBLOCK, //burning furnace
	0, //sign
	0, //door
	0, //ladder
	0, //rail
	RTB_CEMENT, //stone stairs
	0, //wall sign
	0, //lever
	0, //pressure plate
	0, //iron door
	0, //wooden pressure plate
		RTB_GRAVEL, //redstone ore
		RTB_GRAVEL, //lit redstone ore
	0, //redstone torch off
	0, //redstone torch on
	0, //stone button
		0, //snow layer
		RTB_ALUMINIUM, //ice
	RTB_ALUMINIUM, //snow
	0, //cactus
		RTB_BRICK, //clay
	0, //reeds
	RTB_WOODPLANK, //jukebox
	0, //fence
	0, //pumpkin
	RTB_BRICK, //netherrack
		RTB_WOODPLANK, //soul sand
	RTB_GOLD, //glowstone
	RTB_WATER, //portal
	0, //lit pumpkin
	0, //cake
	0, //repeater unpowered
	0, //repeater powered
	0, //stained glass
	0, //trapdoor
	RTB_GRANITE, //monster egg
	RTB_CINDERBLOCK, //stone brick
		0, //huge brown mushroom
		0, //huge red mushroom
	0, //iron bars
	0, //glass pane
	0, //melon
	0, //pumpkin stem
	0, //melon stem
	0, //vine
	0, //fence gate
		RTB_BRICK, //brick stairs
		RTB_CINDERBLOCK, //stone brick stairs
	RTB_GRASS, //mycelium
	0, //lily pad
		RTB_BRICK, //nether brick
		0, //nether brick fence
		RTB_BRICK, //nether brick stairs
	0, //netherwart
		0, //enchanting table
	0, //brewing stand
	RTB_IRON, // cauldron
	RTB_WATER, //end portal
		RTB_ALUMINIUM, //end portal block
		RTB_SAND, //endstone
	RTB_ASPHALT, //dragon egg
		RTB_IRON, //redstone lamp off
		RTB_GOLD, //redstone lamp on
	RTB_WOODPLANK, //double wood slab
	RTB_WOODPLANK, //wood slab
	0, //cocoa bean
	RTB_SAND, //sandstone stairs
		RTB_GRAVEL, //emerald ore
	RTB_ASPHALT, //ender chest
	0, //tripwire hook
	0, //tripwire
	RTB_BLUEPLASTIC, //emerald block
	RTB_WOODPLANK, //spruce stairs
	RTB_WOODPLANK, //birch stairs
	RTB_WOODPLANK, //jungle stairs
		RTB_REDPLASTIC, //command block
	RTB_ALUMINIUM, //beacon
	0, //cobblestone wall
	0, //flower pot
	0, //carrots
	0, //potatoes
	0, //wooden button
	0, //mob head
	RTB_IRON, //anvil
	RTB_WOODPLANK, //trapped chest
	0, //gold pressure plate
	0, //iron pressure plate
	0, //comparator
	0, //nothing
	0, //daylight detector
	RTB_REDPLASTIC, //restone block
		RTB_BRICK, //quartz ore
		RTB_IRON, //hopper
	RTB_ALUMINIUM, //quartz block
	RTB_ALUMINIUM, //quartz slab
	0, //activator rail
	RTB_CINDERBLOCK, //dropper
		RTB_BRICK, //stained hardened clay
	0, //stained glass pane
		0, //new leaves
	RTB_WOODLOG, //new logs
	RTB_WOODPLANK, //acacia stairs
	RTB_WOODPLANK, //dark oak stairs
	0, //nothing
	0, //nothing
	0, //nothing
	0, //nothing
	0, //nothing
	RTB_SAND, //hay
	0, //carpet
		RTB_BRICK, //hardened clay
	RTB_ASPHALT, //coal block
		RTB_ALUMINIUM, //packed ice
	0 //large flower
};


/*
const static char blockMappings[] = {
	0, //air
	4, //stone
	1, //grass
	1, //dirt
	14, //cobblestone
	9, //wood planks
		18, //saplings
	5, //bedrock
	17, //water
	17, //water still
		15, //lava
		15, //lava still
	2, //sand
	11, //gravel
		4, //gold ore
		4, //iron ore
		4, //coal ore
	10, //wood
		19, //leaves
	2, //sponge
		20, //glass
		4, //lapis ore
	16, //lapis
	14, //dispenser
	2, //sandstone
	9, //note block
	0, //bed
	0, //powered rail
	0, //detector rail
	14, //sticky piston
	0, //cobweb
	0, //tall grass
	0, //dead bush
	14, //piston
	0, //piston head
	21, //wool
	0, //piston move
	0, //yellow flower
	0, //red flower
	0, //brown mushroom
	0, //red mushroom
	8, //gold block
	6, //iron block
	14, //double stone slab
	14, //stone slab
	3, //bricks
	15, //tnt
	9, //bookshelf
	13, //mossy cobblestone
	5, //obsidian
	0, //torch
	0, //fire
	0, //spawner
	9, //stairs
	9, //chest
	0, //redstone wire
		4, //diamond ore
	7, //diamond block
	9, //crafting table
	0, //wheat
	1, //farmland
	14, //furnace
	14, //burning furnace
	0, //sign
	22, //door
	0, //ladder
	0, //rail
	14, //stone stairs
	0, //wall sign
	0, //lever
	0, //pressure plate
	23, //iron door
	0, //wooden pressure plate
		4, //redstone ore
		4, //lit redstone ore
	0, //redstone torch off
	0, //redstone torch on
	0, //stone button
		0, //snow layer
	24, //ice
	25, //snow
	0, //cactus
		26, //clay
	0, //reeds
	9, //jukebox
	0, //fence
	27, //pumpkin
	28, //netherrack
	29, //soul sand
	8, //glowstone
	0, //portal
	27, //lit pumpkin
	0, //cake
	0, //repeater unpowered
	0, //repeater powered
	30, //stained glass
	0, //trapdoor
	4, //monster egg
	12, //stone brick
		0, //huge brown mushroom
		0, //huge red mushroom
	31, //iron bars
	32, //glass pane
	33, //melon
	0, //pumpkin stem
	0, //melon stem
	0, //fine
	0, //fence gate
		3, //brick stairs
		12, //stone brick stairs
	1, //mycelium
	0, //lily pad
		0, //nether brick
		0, //nether brick fence
		0, //nether brick stairs
	0, //netherwater
		0, //enchanting table
	0, //brewing stand
	6, // cauldron
	0, //end portal
		0, //end portal block
		2, //endstone
	0, //dragon egg
		0, //redstone lamp off
		0, //redstone lamp on
	9, //double wood slab
	9, //wood slab
	0, //cocoa bean
	2, //sandstone stairs
		4, //emerald ore
	0, //ender chest
	0, //tripwire hook
	0, //tripwire
		0, //emerald block
	9, //spruce stairs
	9, //birch stairs
	9, //jungle stairs
		15, //command block
	0, //beacon
	0, //cobblestone wall
	0, //flower pot
	0, //carrots
	0, //potatoes
	0, //wooden button
	0, //mob head
	0, //anvil
	9, //trapped chest
	0, //gold pressure plate
	0, //iron pressure plate
	0, //comparator
	0, //nothing
	0, //daylight detector
	15, //restone block
		0, //quartz ore
		6, //hopper
	7, //quartz block
	7, //quartz slab
	0, //activator rail
	12, //dropper
		0, //stained hardened clay
	0, //stained glass pane
		0, //new leaves
	10, //new logs
	9, //acacia stairs
	9, //dark oak stairs
	0, //nothing
	0, //nothing
	0, //nothing
	0, //nothing
	0, //nothing
	2, //hay
	0, //carpet
		0, //hardened clay
	5, //coal block
		0, //packed ice
	0 //large flower
};
*/
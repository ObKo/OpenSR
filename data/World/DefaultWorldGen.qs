var context = World.Manager.context;

var sector = World.Sector(context);
sector.name = "Caragon"

var system = World.PlanetarySystem(sector);
system.name = "Solar"

var planet = World.InhabitedPlanet(system);
planet.name = "Earth"

World.Manager.saveWorld("/tmp/test.osr");
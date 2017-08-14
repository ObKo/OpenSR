var context = World.context;

function genRace(idName, name, icon, color, sound) {
    var style = World.RaceStyle();

    style.icon = icon;
    style.color = color;
    style.sound = sound;

    var race = World.Race(context);
    race.objectName = idName;
    race.name = name;
    race.style = style;

    return race
}

function genSector(idName, name, position) {
    var sector = World.Sector(context);
    sector.objectName = idName;
    sector.name = name;
    return sector;
}

function genSystem(idName, sector, name, size, star, starColor, bg) {
    var style = World.PlanetarySystemStyle();

    style.background = bg;
    style.star = star;
    style.starColor = starColor;

    var system = World.PlanetarySystem(sector);
    system.objectName = idName;
    system.name = name;
    system.size = size;
    system.style = style;
    return system;
}

var asteroidStyles = {};

function createAsteroidStyles() {
    var datStyles = Engine.datValue("Data.SE.Asteroid");
    for (var k in datStyles)
    {
        style = World.AsteroidStyle();
        style.texture = "dat:/" + datStyles[k]["Image"];
        style.color = "#FFFFFF";
        asteroidStyles[k] = style;
    }
}

genRace("Race.People", "Race.Name.People", "dat:/Bm.Race.Emblem.2People", "#0030BD", "")
genRace("Race.Feyan", "Race.Name.Fei", "dat:/Bm.Race.Emblem.2Fei", "#E03BFF", "")
genRace("Race.Gaal", "Race.Name.Gaal", "dat:/Bm.Race.Emblem.2Gaal", "#FFDB0A", "")
genRace("Race.Peleng", "Race.Name.Peleng", "dat:/Bm.Race.Emblem.2Peleng", "#EF0000", "")
genRace("Race.Maloq", "Race.Name.Maloc", "dat:/Bm.Race.Emblem.2Maloc", "#607F16", "")
genRace("Race.Blazer", "Race.Name.Kling", "dat:/Bm.Race.Emblem.2Blazer", "#44BBA1", "")
genRace("Race.Keller", "Race.Name.Kling", "dat:/Bm.Race.Emblem.2Keller", "#44BBA1", "")
genRace("Race.Terron", "Race.Name.Kling", "dat:/Bm.Race.Emblem.2Terron", "#44BBA1", "")

createAsteroidStyles();

sector = genSector("Sector.Caragon", "Constellations.Name.3")
var system = genSystem("System.Solar", sector, "Star.02", 10000, "dat:/Bm.Star.00a", "#FFFFFF", "dat:/Bm.BGO.bg00")

var styleCount = Object.keys(asteroidStyles).length
var i = 0
for(var k in asteroidStyles)
{
    var asteroid = World.Asteroid(system);
    asteroid.objectName = "Asteroid." + k;
    asteroid.style = asteroidStyles[k];
    asteroid.period = 15;
    asteroid.semiAxis.x = 1500 + i * 10;
    asteroid.semiAxis.y = 1000;
    asteroid.angle = (i / styleCount) * 2 * 3.1415;
    i++
}

var planet = World.InhabitedPlanet(system);
planet.name = "PlanetName.Solar.2"

context.currentSystem = system

World.saveWorld("/tmp/test.osr");

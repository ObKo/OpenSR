var ASTEROID_STYLE_TYPES = {"": 15, "Blue": 19, "Green": 15, "Red": 14, "Yellow": 8};

var context = world.context;

function genRace(name, icon, color, sound) {
    var style = World.RaceStyle();

    style.icon = icon;
    style.color = color;
    style.sound = sound;

    var race = World.Race(context);
    race.name = name;
    race.style = style;

    return race
}

function genSector(name, position) {
    var sector = World.Sector(context);
    sector.name = name;
    return sector;
}

function genSystem(sector, name, size, star, starColor, bg) {
    var style = World.PlanetarySystemStyle();

    style.background = bg;
    style.star = star;
    style.starColor = starColor;

    var system = World.PlanetarySystem(sector);
    system.name = name;
    system.size = size;
    system.style = style;
    return system;
}

var asteroidStyles = [];

function createAsteroidStyles() {
    for (var k in ASTEROID_STYLE_TYPES)
    {
        for (var i = 0; i < ASTEROID_STYLE_TYPES[k]; i++)
        {
            var num = ("0" + i).slice(-2);
            var path = "res:/DATA/Asteroid/" + k + num + ".gai";

            style = World.AsteroidStyle();
            style.texture = path;
            style.color = "#FFFFFF";

            asteroidStyles.push(style);
        }
    }
}

genRace("People", "res:/DATA/Race/2EmPeople.png", "#0030BD", "")
genRace("Faeyan", "res:/DATA/Race/2EmFei.png", "#E03BFF", "")
genRace("Gaal", "res:/DATA/Race/2EmGaal.png", "#FFDB0A", "")
genRace("Peleng", "res:/DATA/Race/2EmPeleng.png", "#EF0000", "")
genRace("Maloq", "res:/DATA/Race/2EmMaloc.png", "#607F16", "")
genRace("Blazer", "res:/DATA/Race/2EmBlazer.png", "#44BBA1", "")
genRace("Keller", "res:/DATA/Race/2EmKeller.png", "#44BBA1", "")
genRace("Terron", "res:/DATA/Race/2EmTerron.png", "#44BBA1", "")

createAsteroidStyles();

sector = genSector("Caragon")
var system = genSystem(sector, "Solar", 10000, "res:/DATA/Star/Star00.gai", "#FFFFFF", "res:/DATA/BGObj/bg00.gai")

for(var i = 0; i < asteroidStyles.length; i++)
{
    var asteroid = World.Asteroid(system);
    asteroid.style = asteroidStyles[i];
    asteroid.position.x = (Math.floor(i % 9) - 4) * 50;
    asteroid.position.y = (Math.floor(i / 9) - 3) * 50;
}

var planet = World.InhabitedPlanet(system);
planet.name = "Earth"

context.currentSystem = system

world.saveWorld("/tmp/test.osr");

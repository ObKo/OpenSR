var context = World.context;

function genRace(idName, name, icon, color, sound) {
    var style = World.newRaceStyle();

    style.icon = icon;
    style.color = color;
    style.sound = sound;

    var race = World.newRace(context);
    race.objectName = idName;
    race.name = name;
    race.style = style;

    return race
}

function genSector(idName, name, position) {
    var sector = World.newSector(context);
    sector.objectName = idName;
    sector.name = name;
    return sector;
}

function genSystem(idName, sector, name, size, star, starColor, bg) {
    var style = World.newPlanetarySystemStyle();

    style.background = bg;
    style.star = star;
    style.starColor = starColor;

    var system = World.newPlanetarySystem(sector);
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
        style = World.newAsteroidStyle();
        style.texture = "dat:/" + datStyles[k]["Image"];
        style.color = "#FFFFFF";
        asteroidStyles[k] = style;
    }
}

function getPlanetStyleFromDat(id) {
    var planet = Engine.datValue("Data.SE.Planet." + id)
    var color;
    if ("AtmColor" in planet) {
        var rgb = planet["AtmColor"].split(',')
        color = Qt.rgba(rgb[0] / 255.0, rgb[1] / 255.0, rgb[2] / 255.0, 1.0).toString()
    }
    else
        color = Qt.rgba(1.0, 1.0, 1.0, 1.0).toString()

    var splitted, cloud0 = "", cloud1 = "", cloud0Speed = 1.0, cloud1Speed = 1.0
    if ("Cloud0" in planet) {
        splitted = planet["Cloud0"].split(',')
        cloud0 = "dat:/" + splitted[1]
        cloud0Speed = parseFloat(splitted[0])
    }
    if ("Cloud1" in planet) {
        splitted = planet["Cloud1"].split(',')
        cloud1 = "dat:/" + splitted[1]
        cloud1Speed = parseFloat(splitted[0])
    }
    
    var style = World.newPlanetStyle();
    style.surface  = "dat:/" + planet["Image"];
    style.cloud0 = cloud0;
    style.cloud1 = cloud1;
    style.atmosphere = color
    style.radius = planet["Radius"];
    
    return style
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
    var asteroid = World.newAsteroid(system);
    asteroid.objectName = "Asteroid." + k;
    asteroid.style = asteroidStyles[k];
    asteroid.period = 15;
    asteroid.semiAxis.x = 1500 + i * 10;
    asteroid.semiAxis.y = 1000;
    asteroid.angle = (i / styleCount) * 2 * 3.1415;
    i++
}

var planet = World.newInhabitedPlanet(system);
planet.name = "PlanetName.Solar.2"
planet.style = getPlanetStyleFromDat("200")
planet.period = 15;
planet.angle = 3.1415 / 4;
planet.position = Qt.point(355, -222);

function initStation(obj, kind) {
    switch (kind) {
    case "ranger":
    case "rangerCenter":
    case "rc":
        obj.stationKind = World.SpaceStation.RangerCenter;
        obj.style = World.newStationStyle();
        obj.style.texture = "res:/DATA/Ruins/RC.gai";
    }
}

var rangerCenter1 = World.newSpaceStation(system);
initStation(rangerCenter1, "ranger")
//console.log("rangerCenter1.style = " + rangerCenter1.style);
rangerCenter1.position = Qt.point(-400, -100);

context.currentSystem = system;

World.saveWorld("/tmp/test.osr");

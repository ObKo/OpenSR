var context = World.Manager.context;

function genRace(name, icon, color, sound) {
    var race = new World.Race(context);
    race.name = name;
    race.style = World.RaceStyle(icon, color, sound);
    return race
}

function genSector(name, position) {
    var sector = new World.Sector(context);
    sector.name = name;
    return sector;
}

genRace("People", "res:/DATA/Race/2EmPeople.png", "#0030BD", "")
genRace("Faeyan", "res:/DATA/Race/2EmFei.png", "#E03BFF", "")
genRace("Gaal", "res:/DATA/Race/2EmGaal.png", "#FFDB0A", "")
genRace("Peleng", "res:/DATA/Race/2EmPeleng.png", "#EF0000", "")
genRace("Maloq", "res:/DATA/Race/2EmMaloc.png", "#607F16", "")
genRace("Blazer", "res:/DATA/Race/2EmBlazer.png", "#44BBA1", "")
genRace("Keller", "res:/DATA/Race/2EmKeller.png", "#44BBA1", "")
genRace("Terron", "res:/DATA/Race/2EmTerron.png", "#44BBA1", "")

sector = genSector("Caragon")

var system = new World.PlanetarySystem(sector);
system.name = "Solar"

var planet = new World.InhabitedPlanet(system);
planet.name = "Earth"

World.Manager.saveWorld("/tmp/test.osr");
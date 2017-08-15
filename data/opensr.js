Engine.addDATFile("res:/main.dat")
Engine.addDATFile("res:/rus.dat")
Engine.addDATFile("res:/CacheData.dat", true)

var dir = Engine.dataDir
Engine.resources.addPKGArchive(dir + "/2captain.pkg")
Engine.resources.addPKGArchive(dir + "/2gov.pkg")
Engine.resources.addPKGArchive(dir + "/2Items.pkg")
Engine.resources.addPKGArchive(dir + "/1main.pkg")
Engine.resources.addPKGArchive(dir + "/2main.pkg")
Engine.resources.addPKGArchive(dir + "/common.pkg")
Engine.resources.addPKGArchive(dir + "/ShipFull.pkg")
Engine.resources.addPKGArchive(dir + "/ShipSmall.pkg")
Engine.resources.addPKGArchive(dir + "/Sound.pkg")
Engine.resources.addPKGArchive(dir + "/Star.pkg")

Engine.loadPlugin("world")
Engine.showQMLComponent("qrc:/OpenSR/MainMenu.qml")

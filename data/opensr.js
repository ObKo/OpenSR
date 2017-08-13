engine.addDATFile("res:/main.dat")
engine.addDATFile("res:/rus.dat")
engine.addDATFile("res:/CacheData.dat", true)

var dir = engine.dataDir
engine.resources.addPKGArchive(dir + "/2captain.pkg")
engine.resources.addPKGArchive(dir + "/2gov.pkg")
engine.resources.addPKGArchive(dir + "/2Items.pkg")
engine.resources.addPKGArchive(dir + "/2main.pkg")
engine.resources.addPKGArchive(dir + "/common.pkg")
engine.resources.addPKGArchive(dir + "/ShipFull.pkg")
engine.resources.addPKGArchive(dir + "/ShipSmall.pkg")
engine.resources.addPKGArchive(dir + "/Sound.pkg")
engine.resources.addPKGArchive(dir + "/Star.pkg")

engine.loadPlugin("world")
engine.showQMLComponent("qrc:/OpenSR/MainMenu.qml")

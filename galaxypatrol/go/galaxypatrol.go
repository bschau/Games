package main

import (
	tl "github.com/JoelOtter/termloop"
)

var hud *Hud
var score int
var fuel int
var game *tl.Game

func main() {
	score = 0
	fuel = 50
	game = ZX81Init()
	game.Screen().SetFps(5)

	buildLevel()
	game.Start()
}

func buildLevel() {
	level := ZX81GetLevel()
	game.Screen().SetLevel(level)

	hud = HudNew()
	level.AddEntity(hud.score)
	level.AddEntity(hud.fuel)

	StarsAdd(level)
	PlayerNew(level)

	TimerAdd(level)
}

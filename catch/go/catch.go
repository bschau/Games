package main

import (
	"github.com/JoelOtter/termloop"
	"github.com/nsf/termbox-go"
)

var score int
var hud *Hud
var game *termloop.Game
var delay int64

func main() {
	score = 0
	delay = 300
	game = ZX81Init()
	termbox.HideCursor()

	buildLevel()
	game.Start()
}

func buildLevel() {
	level := ZX81GetLevel()
	game.Screen().SetLevel(level)
	bucket := BucketNew()
	level.AddEntity(bucket.Entity)

	shield := ShieldNew(bucket.bucketX)
	level.AddEntity(shield.Entity)

	level.AddEntity(termloop.NewText(0, 21, "════════════════════════════════", termloop.ColorBlack, termloop.ColorWhite))
	hud = HudNew()
	level.AddEntity(hud.Text)

	level.AddEntity(BallNew(bucket, shield))
}

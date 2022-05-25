package main

import (
	"fmt"
	"math/rand"
	"time"

	tl "github.com/JoelOtter/termloop"
	"github.com/nsf/termbox-go"
)

// SeededRand is the seeded randomizer
var SeededRand *rand.Rand

const ScreenW = 32
const ScreenH = 22

// ZX81Init - initialize the display and other support
func ZX81Init() *tl.Game {
	s1 := rand.NewSource(time.Now().UnixNano())
	SeededRand = rand.New(s1)

	termbox.Init()
	width, height := termbox.Size()
	if width < ScreenW || height < ScreenH {
		panic(fmt.Sprintf("Please resize screen to at least %d x %d characters", ScreenW, ScreenH))
	}

	termbox.HideCursor()
	return tl.NewGame()
}

func ZX81GetLevel() *tl.BaseLevel {
	level := tl.NewBaseLevel(tl.Cell{
		Bg: tl.ColorBlack,
		Fg: tl.ColorBlack,
		Ch: ' ',
	})
	level.AddEntity(tl.NewRectangle(0, 0, ScreenW, ScreenH, tl.ColorWhite))
	return level
}

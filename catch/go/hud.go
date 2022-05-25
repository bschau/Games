package main

import (
	"fmt"

	"github.com/JoelOtter/termloop"
)

type Hud struct {
	*termloop.Text
}

func HudNew() *Hud {
	hud := &Hud{
		termloop.NewText(0, 0, getScore(), termloop.ColorBlack, termloop.ColorWhite),
	}
	return hud
}

func HudUpdate(hud *Hud, score int) {
	hud.SetText(getScore())
}

func getScore() string {
	return fmt.Sprintf("Score: %d", score)
}

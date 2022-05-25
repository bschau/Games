package main

import (
	"fmt"

	tl "github.com/JoelOtter/termloop"
)

type Hud struct {
	score *tl.Text
	fuel  *tl.Text
}

func HudNew() *Hud {
	fuelText := getFuel()
	fuelPos := ScreenW - len(fuelText)
	hud := &Hud{
		tl.NewText(0, 0, getScore(), tl.ColorBlack, tl.ColorWhite),
		tl.NewText(fuelPos, 0, fuelText, tl.ColorBlack, tl.ColorWhite),
	}
	return hud
}

func HudUpdate(hud *Hud, score int) {
	hud.score.SetText(getScore())
	hud.fuel.SetText(getFuel())
}

func getScore() string {
	return fmt.Sprintf("Score: %d", score)
}

func getFuel() string {
	return fmt.Sprintf("Fuel: %d", fuel)
}

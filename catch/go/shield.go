package main

import (
	"github.com/JoelOtter/termloop"
)

type Shield struct {
	*termloop.Entity
	shieldX int
	shieldY int
}

func ShieldNew(bucketX int) *Shield {
	shieldCanvas := termloop.Canvas{
		{termloop.Cell{Bg: termloop.ColorWhite, Fg: termloop.ColorBlack, Ch: '═'}},
		{termloop.Cell{Bg: termloop.ColorWhite, Fg: termloop.ColorBlack, Ch: '═'}},
		{termloop.Cell{Bg: termloop.ColorWhite, Fg: termloop.ColorBlack, Ch: '═'}},
	}

	shieldX := SeededRand.Intn(4) - 3 + bucketX - 1
	if shieldX < 3 {
		shieldX = 3
	} else if shieldX > 29 {
		shieldX = 29
	}

	shieldY := SeededRand.Intn(8) + 10

	shield := &Shield{
		termloop.NewEntityFromCanvas(shieldX, shieldY, shieldCanvas),
		shieldX,
		shieldY,
	}

	return shield
}

func ShieldHit(shield *Shield, ballX int, ballY int) bool {
	return ballX >= shield.shieldX &&
		ballX <= shield.shieldX+2 &&
		ballY == shield.shieldY
}

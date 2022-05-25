package main

import (
	tl "github.com/JoelOtter/termloop"
)

type Star struct {
	*tl.Entity
}

var stars []*Star

func StarsAdd(level *tl.BaseLevel) {
	star := getStar(1, 15)
	stars = append(stars, star)
	level.AddEntity(star)

	star = getStar(2, 15)
	stars = append(stars, star)
	level.AddEntity(star)

	for i := 3; i < ScreenH; i++ {
		star = getStar(i, 1000)
		stars = append(stars, star)
		level.AddEntity(star)
	}
}

func getStar(y int, hit int) *Star {
	x := hit
	for x == hit {
		x = SeededRand.Intn(ScreenW)
	}

	star := &Star{
		tl.NewEntity(x, y, 1, 1),
	}
	star.SetCell(0, 0, &tl.Cell{Fg: tl.ColorBlack, Bg: tl.ColorWhite, Ch: '*'})
	return star
}

func StarsUpdate() {
	for i := 0; i < len(stars); i++ {
		x, y := stars[i].Position()
		y--
		if y < 1 {
			y = 21
			x = SeededRand.Intn(ScreenW)
		}
		stars[i].SetPosition(x, y)
	}
}

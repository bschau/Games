package main

import (
	"github.com/JoelOtter/termloop"
	tl "github.com/JoelOtter/termloop"
	"github.com/nsf/termbox-go"
)

type Player struct {
	*tl.Entity
	x int
}

var player *Player
var prevKey tl.Key

func PlayerNew(level *tl.BaseLevel) {
	prevKey = 0
	player = &Player{
		tl.NewEntity(ScreenW/2, 1, 1, 1),
		ScreenW / 2,
	}
	player.SetCell(0, 0, &tl.Cell{Fg: tl.ColorBlack, Bg: tl.ColorWhite, Ch: 'V'})
	level.AddEntity(player)
}

func PlayerUpdate() {
	if player.x > 0 {
		player.x -= 1
	}
	player.SetPosition(player.x, 1)
}

func (p *Player) Tick(ev tl.Event) {
	if ev.Type == tl.EventResize {
		termbox.HideCursor()
		return
	}

	if ev.Type != termloop.EventKey {
		return
	}

	if ev.Key == tl.KeySpace {
		if prevKey == ev.Key {
			return
		}

		if player.x < 30 {
			player.x += 2
			prevKey = ev.Key
		}
	}
}

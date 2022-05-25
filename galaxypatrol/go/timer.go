package main

import (
	tl "github.com/JoelOtter/termloop"
)

type Timer struct {
	*tl.Entity
}

func TimerAdd(level *tl.BaseLevel) {
	timer := &Timer{
		tl.NewEntity(0, 0, 0, 0),
	}
	level.AddEntity(timer)
}

func (t *Timer) Tick(ev tl.Event) {
	PlayerUpdate()
	StarsUpdate()
}

package main

import (
	"os"
	"time"

	"github.com/JoelOtter/termloop"
	"github.com/nsf/termbox-go"
)

type Ball struct {
	*termloop.Text
	bucket     *Bucket
	shield     *Shield
	delay      int
	clear      bool
	flashes    int
	lastUpdate int64
	flashing   bool
	newGame    bool
	ballX      int
	ballY      int
}

func BallNew(bucket *Bucket, shield *Shield) *Ball {
	ballX := SeededRand.Intn(32)
	ballY := 1
	ball := &Ball{
		termloop.NewText(ballX, ballY, "֍", termloop.ColorBlack, termloop.ColorWhite),
		bucket,
		shield,
		1,
		false,
		0,
		time.Now().UnixMilli(),
		false,
		false,
		ballX,
		ballY,
	}
	return ball
}

func (b *Ball) Tick(ev termloop.Event) {
	if ev.Type == termloop.EventResize {
		termbox.HideCursor()
		return
	}

	if b.flashing || ev.Type != termloop.EventKey {
		return
	}

	switch ev.Key {
	case termloop.KeyArrowRight:
		if b.ballX < 31 {
			b.ballX += 1
		}
	case termloop.KeyArrowLeft:
		if b.ballX > 0 {
			b.ballX -= 1
		}
	}

	b.SetPosition(b.ballX, b.ballY)
	isGameOver(b)
}

func (b *Ball) Draw(screen *termloop.Screen) {
	now := time.Now().UnixMilli()
	if shouldUpdate(b, now) {
		b.lastUpdate = now
		if b.flashing {
			handleFlashing(b)
		} else {
			b.ballY++
			b.SetPosition(b.ballX, b.ballY)
			isGameOver(b)
		}
	}
	b.Text.Draw(screen)
	termbox.HideCursor()
}

func shouldUpdate(b *Ball, now int64) bool {
	if b.flashing {
		return now-b.lastUpdate > 300
	}

	return now-b.lastUpdate > delay
}

func handleFlashing(b *Ball) {
	b.delay--
	if b.delay > 0 {
		return
	}

	b.delay = 2
	if b.clear {
		b.clear = false
		b.Text.SetText("֍")
	} else {
		b.clear = true
		b.Text.SetText(" ")
	}
	b.flashes--
	if b.flashes > 0 {
		return
	}

	if b.newGame {
		buildLevel()
		return
	}

	os.Exit(0)
}

func isGameOver(b *Ball) {
	if BucketBallCaught(b.bucket, b.ballX, b.ballY) {
		score++
		HudUpdate(hud, score)
		if delay > 0 {
			delay -= 2
		}
		setFlashing(b, true)
		return
	}

	if b.ballY > 20 {
		setFlashing(b, false)
		return
	}

	if ShieldHit(b.shield, b.ballX, b.ballY) {
		setFlashing(b, false)
		return
	}

	if BucketHit(b.bucket, b.ballX, b.ballY) {
		setFlashing(b, false)
	}
}

func setFlashing(b *Ball, newGame bool) {
	b.flashing = true
	b.newGame = newGame

	if b.newGame {
		b.flashes = 4
	} else {
		b.flashes = 12
	}

	b.delay = 1
	b.clear = false
}

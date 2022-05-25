package main

import (
	"github.com/JoelOtter/termloop"
)

type Bucket struct {
	*termloop.Entity
	bucketX int
}

func BucketNew() *Bucket {
	bucketCanvas := termloop.Canvas{
		{termloop.Cell{Bg: termloop.ColorWhite, Fg: termloop.ColorBlack, Ch: '║'}, termloop.Cell{Bg: termloop.ColorWhite, Fg: termloop.ColorBlack, Ch: '╚'}},
		{termloop.Cell{Bg: termloop.ColorWhite, Fg: termloop.ColorBlack, Ch: ' '}, termloop.Cell{Bg: termloop.ColorWhite, Fg: termloop.ColorBlack, Ch: '═'}},
		{termloop.Cell{Bg: termloop.ColorWhite, Fg: termloop.ColorBlack, Ch: '║'}, termloop.Cell{Bg: termloop.ColorWhite, Fg: termloop.ColorBlack, Ch: '╝'}},
	}
	bucketX := SeededRand.Intn(29)

	bucket := &Bucket{
		termloop.NewEntityFromCanvas(bucketX, 19, bucketCanvas),
		bucketX,
	}
	return bucket
}

func BucketBallCaught(b *Bucket, x int, y int) bool {
	return x == b.bucketX+1 && y == 19
}

func BucketHit(b *Bucket, x int, y int) bool {
	return x >= b.bucketX && x <= b.bucketX+2 &&
		y >= 19 && y <= 20
}

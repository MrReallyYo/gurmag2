package fishing

import (
	"image"
)

type BobberFinder interface {
	Find(region image.Rectangle) image.Point
}

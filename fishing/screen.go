package fishing

import (
	"image"
)

type Screen interface {
	Size() (size image.Rectangle)
	Color(location image.Point) (r, g, b int)
	Free()
}















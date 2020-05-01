package fishing

import (
	"github.com/MrReallyYo/gurmag2/bridge"
	"image"
)

func NewBridgeScreen() *BridgeScreen {
	name := "World of Warcraft"
	clazz := "GxWindowClass"

	position, buffer := bridge.GetScreen(name, clazz)

	return &BridgeScreen{
		rect:   position,
		buffer: buffer,
	}
}

type BridgeScreen struct {
	rect   image.Rectangle
	buffer []byte
}

func (s *BridgeScreen) Size() (w, h int) {
	return s.rect.Max.X - s.rect.Min.X, s.rect.Max.Y - s.rect.Min.Y
}

func (s *BridgeScreen) Color(location image.Point) (r, g, b int) {
	w, _ := s.Size()
	pixel := location.X + location.Y*w
	return int(s.buffer[3*pixel]), int(s.buffer[3*pixel+1]), int(s.buffer[3*pixel+2])
}

func (s *BridgeScreen) Free() {

}

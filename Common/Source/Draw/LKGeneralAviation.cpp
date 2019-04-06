/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   $Id: LKGeneralAviation.cpp,v 1.1 2010/12/11 19:06:34 root Exp root $
   $Id: LKGeneralAviation.cpp,v 1.2 2019/03/31 21:12:56 aircaft now at the center of the compass rose $

   LKGeneralAviation.cpp original work by Oren
   Improved by Alberto Realis-Luc
*/

#include "externs.h"
#include "LKObjects.h"
#include "RGB.h"

void drawOutlineText(LKSurface& Surface, int x, int y, const TCHAR * textBuffer) {
#ifdef USE_FREETYPE
#warning "to slow, rewrite using freetype outline"
#endif
	Surface.SetTextColor(RGB_BLACK);
	Surface.DrawText(x -1, y -1, textBuffer);
	Surface.DrawText(x +1, y +1, textBuffer);
	Surface.DrawText(x -1, y   , textBuffer);
	Surface.DrawText(x   , y +1, textBuffer);
	Surface.SetTextColor(RGB_WHITE);
	Surface.DrawText(x, y, textBuffer);
}

void MapWindow::DrawGAscreen(LKSurface& Surface, const POINT& AircraftPos, const RECT& rc) {

	// Only for Trackup and compatible modes
	if (DisplayOrientation == NORTHSMART ||
		DisplayOrientation == NORTHTRACK ||
		DisplayOrientation == NORTHUP ||
		MapWindow::mode.autoNorthUP() ||
		MapWindow::mode.Is(MapWindow::Mode::MODE_CIRCLING)
	) return;

	// Print current heading
	TCHAR textBuffer[4];
	const auto oldfont = Surface.SelectObject(LK8InfoNormalFont); // always remember to save object or we miss font
	const int hdg = (int)round(DrawInfo.TrackBearing);
	_stprintf(textBuffer,_T("%03d"), hdg);
	SIZE textSize;
	Surface.GetTextSize(textBuffer, &textSize); // get size of heading printed digits
	const int halfBrgSize = textSize.cx/2;
	const int deciBrgSize = textSize.cx/10;
	const int brgYoffset = textSize.cy + (ScreenLandscape ? 0 : 30);
	drawOutlineText(Surface, AircraftPos.x - halfBrgSize, ScreenLandscape ? 0 : 30, textBuffer);


	// Calculate compass radius: consider the offset below heading pointer
	const int radius = AircraftPos.y - brgYoffset - 12;


	// Draw heading pointer on the top of the compass
	const POINT hdgPointer[7] = {
			{AircraftPos.x - halfBrgSize - 5, brgYoffset - 5},
			{AircraftPos.x - halfBrgSize - 5, brgYoffset + 2},
			{AircraftPos.x - deciBrgSize    , brgYoffset + 2},
			{AircraftPos.x                  , brgYoffset + 7},
			{AircraftPos.x + deciBrgSize,     brgYoffset + 2},
			{AircraftPos.x + halfBrgSize + 5, brgYoffset + 2},
			{AircraftPos.x + halfBrgSize + 5, brgYoffset - 5}
	};
	const auto oldpen=Surface.SelectObject(LKPen_Black_N3);
	Surface.Polyline(hdgPointer,7);
	Surface.SelectObject(LKPen_White_N2);
	Surface.Polyline(hdgPointer,7);

	// Draw black part of 120 degree arc
	static const int rightArc = ScreenLandscape ? 60 : 30;
	static const int leftArc = (int)AngleLimit360(-rightArc);
	Surface.SelectObject(LKPen_Black_N5);
	Surface.DrawArc(AircraftPos.x, AircraftPos.y,radius, rc, leftArc, rightArc);

	// Draw the dents around the circle
	static const int step = ScreenLandscape ? 10 : 5;
	static const int bigStep = ScreenLandscape ? 30 : 10;
	const int diff = (int)round(hdg % step);
	Surface.SelectObject(LK8MediumFont);
	for(int i = -rightArc, screenAngle = leftArc - diff, curHdg = hdg - rightArc - diff; i <= rightArc; i += step, screenAngle += step, curHdg += step) {
		screenAngle = (int)AngleLimit360(screenAngle);
		if (screenAngle < leftArc && screenAngle > rightArc) continue;
		const int displayHeading = (int)AngleLimit360(curHdg);
		int tickLength = radius - 10; // the length of the tickmarks on the compass rose
		if (displayHeading % bigStep == 0) {
			tickLength -= 8; // make longer ticks
			const int drawHdg = displayHeading/10;
			switch (drawHdg) {
			case 0:
				_stprintf(textBuffer, _T("N"));
				break;
			case 9:
				_stprintf(textBuffer, _T("E"));
				break;
			case 18:
				_stprintf(textBuffer, _T("S"));
				break;
			case 27:
				_stprintf(textBuffer, _T("W"));
				break;
			default:
				_stprintf(textBuffer, _T("%d"), drawHdg);
				break;
			}
			Surface.GetTextSize(textBuffer, &textSize);
			const int textX = AircraftPos.x + (int) ((radius - (textSize.cy/2)-2) * fastsine(screenAngle)) - textSize.cx/2;
			const int textY = AircraftPos.y - (int) ((radius - (textSize.cy/2)-2) * fastcosine(screenAngle));
			drawOutlineText(Surface, textX, textY, textBuffer);
		}
		const POINT dent[2] = {
			{ AircraftPos.x + (int)(radius     * fastsine(screenAngle)), AircraftPos.y - (int)(radius     * fastcosine(screenAngle)) },
			{ AircraftPos.x + (int)(tickLength * fastsine(screenAngle)), AircraftPos.y - (int)(tickLength * fastcosine(screenAngle)) }
		};
		Surface.SelectObject(LKPen_Black_N5);
		Surface.Polyline(dent,2);
		Surface.SelectObject(LKPen_White_N2);
		Surface.Polyline(dent,2);
	}

	// Draw white part of the 120 degree arc
	Surface.SelectObject(LKPen_White_N2);
	Surface.DrawArc(AircraftPos.x, AircraftPos.y, radius, rc, leftArc, rightArc);

	// Restore previous text color, pen and font
	Surface.SetTextColor(RGB_BLACK);
	Surface.SelectObject(oldpen);
	Surface.SelectObject(oldfont);
}

/*
 * Stellarium
 * Copyright (C) 2002 Fabien Chereau
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef _CONSTELLATION_H_
#define _CONSTELLATION_H_

#include <vector>
#include <boost/intrusive_ptr.hpp>
#include <QString>

#include "StelObject.hpp"
#include "StelUtils.hpp"
#include "Fader.hpp"
#include "STextureTypes.hpp"

class StarMgr;
class SFont;

//! @class Constellation
//! The Constellation class models a grouping of stars in a Sky Culture.
//! Each Constellation consists of a list of stars identified by their 
//! Hipparcos catalogue numbers, a name and optionally an abbreviated name,
//! boundary shape and an artistic pictorial representation.
class Constellation : public StelObject
{
	friend class ConstellationMgr;
private:
	Constellation();
	~Constellation();
    
	// StelObject method to override
	//! Get a string with data about the Constellation.
	//! Constellations support the following InfoStringGroup flags:
	//! - Name
	//! @param core the Stelore object
	//! @param flags a set of InfoStringGroup items to include in the return value.
	//! @return a QString a description of the Planet.
	virtual QString getInfoString(const StelCore* core, const InfoStringGroup& flags) const
	{
		if (flags&Name) return getNameI18n() + "(" + getShortName() + "°";
		else return "";
	}

	//! Get the module/object type string.
	//! @return "Constellation"
	virtual QString getType(void) const {return "Constellation";}

	//! observer centered J2000 coordinates.
	virtual Vec3d getObsJ2000Pos(const Navigator *nav) const {return XYZname;}

	virtual double getAngularSize(const StelCore* core) const {assert(0); return 0;}; // TODO
	
	//! @param record string containing the following whitespace 
	//! separated fields: abbreviation - a three character abbreviation 
	//! for the constellation, a number of lines, and a list of Hipparcos
	//! catalogue numbers which, when connected form the lines of the 
	//! constellation.
	//! @param starMgr a pointer to the StarManager object.
	//! @return false if can't parse record, else true.
	bool read(const QString& record, StarMgr *starMgr);
	
	//! Draw the constellation name
	void drawName(SFont * constfont, Projector* prj) const;
	//! Draw the constellation art
	void drawArt(Projector* prj, const Navigator* nav) const;
	//! Draw the constellation boundary
	void drawBoundaryOptim(Projector* prj) const;
	
	//! Test if a star is part of a Constellation.
	//! This member tests to see if a star is one of those which make up
	//! the lines of a Constellation.
	//! @return a pointer to the constellation which the star is a part of, 
	//! or NULL if the star is not part of a constellation
	const Constellation* isStarIn(const StelObject*) const;
	
	//! Get the brightest star in a Constellation.
	//! Checks all stars which make up the constellation lines, and returns 
	//! a pointer to the one with the brightest apparent magnitude.
	//! @return a pointer to the brightest star
	StelObjectP getBrightestStarInConstellation(void) const;

	//! Get the translated name for the Constellation.
	QString getNameI18n(void) const {return nameI18;}
	//! Get the English name for the Constellation (returns the abbreviation).
	QString getEnglishName(void) const {return abbreviation;}
	//! Get the short name for the Constellation (returns the abbreviation).
	QString getShortName(void) const {return abbreviation;}
	//! Draw the lines for the Constellation.
	//! This method uses the coords of the stars (optimized for use thru 
	//! the class ConstellationMgr only).
	void drawOptim(Projector* prj) const;
	//! Draw the art texture, optimized function to be called thru a constellation manager only.
	void drawArtOptim(Projector* prj, const Navigator* nav) const;
	//! Update fade levels according to time since various events.
	void update(int deltaTime);
	//! Turn on and off Constellation line rendering.
	//! @param b new state for line drawing.
	void setFlagLines(bool b) {lineFader=b;}
	//! Turn on and off Constellation boundary rendering.
	//! @param b new state for boundary drawing.
	void setFlagBoundaries(bool b) {boundaryFader=b;}
	//! Turn on and off Constellation name label rendering.
	//! @param b new state for name label drawing.
	void setFlagName(bool b) {nameFader=b;}
	//! Turn on and off Constellation art rendering.
	//! @param b new state for art drawing.
	void setFlagArt(bool b) {artFader=b;}
	//! Get the current state of Constellation line rendering.
	//! @return true if Constellation line rendering it turned on, else false.
	bool getFlagLines(void) const {return lineFader;}
	//! Get the current state of Constellation boundary rendering.
	//! @return true if Constellation boundary rendering it turned on, else false.
	bool getFlagBoundaries(void) const {return boundaryFader;}
	//! Get the current state of Constellation name label rendering.
	//! @return true if Constellation name label rendering it turned on, else false.
	bool getFlagName(void) const {return nameFader;}
	//! Get the current state of Constellation art rendering.
	//! @return true if Constellation art rendering it turned on, else false.
	bool getFlagArt(void) const {return artFader;}
	
	//! International name (translated using gettext)
	QString nameI18;
	//! Name in english
	QString englishName;
	//! Name in native language
	QString nativeName;
	//! Abbreviation (of the latin name for western constellations)
	QString abbreviation;
	//! Direction vector pointing on constellation name drawing position 
	Vec3f XYZname;
	Vec3d XYname;
	//! Number of segments in the lines
	unsigned int numberOfSegments;
	//! List of stars forming the segments
	StelObjectP* asterism;
	STextureSP artTexture;
	Vec3d artVertex[9];
	
	//! Define whether art, lines, names and boundary must be drawn
	LinearFader artFader, lineFader, nameFader, boundaryFader;
	vector<vector<Vec3f> *> isolatedBoundarySegments;
	vector<vector<Vec3f> *> sharedBoundarySegments;
	
	//! Currently we only need one color for all constellations, this may change at some point
	static Vec3f lineColor;
	static Vec3f labelColor;
	static Vec3f boundaryColor;

	static bool singleSelected;
};

#endif // _CONSTELLATION_H_

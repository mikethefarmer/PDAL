/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#include <cassert>
#include <iostream>

#include "libpc/FauxReader.hpp"
#include "libpc/Utils.hpp"

using std::vector;
using std::string;
using std::cout;

namespace libpc
{

FauxReader::FauxReader(const Bounds<double>& bounds, int numPoints, Mode mode)
    : Reader()
    , m_mode(mode)
{
    Header* header = new Header;
    Schema& schema = header->getSchema();

    schema.addDimension(Dimension(Dimension::Field_X, Dimension::Float));
    schema.addDimension(Dimension(Dimension::Field_Y, Dimension::Float));
    schema.addDimension(Dimension(Dimension::Field_Z, Dimension::Float));
    schema.addDimension(Dimension(Dimension::Field_GpsTime, Dimension::Uint64));

    header->setNumPoints(numPoints);
    header->setBounds(bounds);

    setHeader(header);

    return;
}

FauxReader::FauxReader(const Bounds<double>& bounds, int numPoints, Mode mode, const std::vector<Dimension>& dimensions)
    : Reader()
    , m_mode(mode)
{
    Header* header = new Header;

    Schema& schema = header->getSchema();
    if (dimensions.size() == 0)
    {
        throw; // BUG
    }
    schema.addDimensions(dimensions);

    header->setNumPoints(numPoints);
    header->setBounds(bounds);

    setHeader(header);

    return;
}


boost::uint32_t FauxReader::readPoints(PointData& data)
{
    // make up some data and put it into the buffer

    const boost::uint32_t numPoints = data.getNumPoints();
    assert(m_currentPointIndex + numPoints <= getHeader().getNumPoints());

    const SchemaLayout& schemaLayout = data.getSchemaLayout();
    const Schema& schema = schemaLayout.getSchema();
    Header& header = getHeader();

    const Bounds<double>& bounds = header.getBounds();
    const std::vector< Range<double> >& dims = bounds.dimensions();
    const double minX = dims[0].getMinimum();
    const double maxX = dims[0].getMaximum();
    const double minY = dims[1].getMinimum();
    const double maxY = dims[1].getMaximum();
    const double minZ = dims[2].getMinimum();
    const double maxZ = dims[2].getMaximum();

    const std::size_t offsetT = schema.getDimensionIndex(Dimension::Field_GpsTime);
    const std::size_t offsetX = schema.getDimensionIndex(Dimension::Field_X);
    const std::size_t offsetY = schema.getDimensionIndex(Dimension::Field_Y);
    const std::size_t offsetZ = schema.getDimensionIndex(Dimension::Field_Z);

    boost::uint64_t time = m_currentPointIndex;

    for (boost::uint32_t pointIndex=0; pointIndex<numPoints; pointIndex++)
    {
        float x;
        float y;
        float z;
        if (m_mode == Random)
        {
            x = (float)Utils::random(minX, maxX);
            y = (float)Utils::random(minY, maxY);
            z = (float)Utils::random(minZ, maxZ);
        }
        else
        {
            x = (float)minX;
            y = (float)minY;
            z = (float)minZ;
        }

        data.setValid(pointIndex);

        data.setField<float>(pointIndex, offsetX, x);
        data.setField<float>(pointIndex, offsetY, y);
        data.setField<float>(pointIndex, offsetZ, z);
        data.setField<boost::uint64_t>(pointIndex, offsetT, time);

        ++time;
    }

    m_currentPointIndex += numPoints;
    m_numPointsRead += numPoints;

    return numPoints;
}


void FauxReader::seekToPoint(boost::uint64_t pointNumber)
{
    m_currentPointIndex = pointNumber;
}


} // namespace libpc


/*
 * CodeQuery
 * Copyright (C) 2013-2016 ruben2020 https://github.com/ruben2020/
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "asrt.h"
#include "core_iface.h"
#include "showgraph.h"

/** Magnifying factor for getting more detailed images */
const int IMAGE_EXPORT_SCALE_FACTOR = 2;

/** Adjust value for image's bounding rectangle on scene rendering */
const qreal IMAGE_RECT_ADJUST = 10;

QImage showgraph::convertToImage(QString grpxml)
{

    /** Create graph instance */
    GraphView* graph_view = new GraphView();
    graph_view->setGraph( new GGraph( graph_view, true));

    /** Read graph from XML */
    graph_view->graph()->readFromXML( grpxml);
 
    /**
     * Perform layout in single thread.
     * Multi thread wouldn't work since we do not run QApplication::exec() and there is not event loop
     */
    graph_view->graph()->doLayoutSingle();

    /** Get scene rectangle */
    QRectF scene_rect( graph_view->scene()->itemsBoundingRect()
                       .adjusted( -IMAGE_RECT_ADJUST, -IMAGE_RECT_ADJUST,
                                   IMAGE_RECT_ADJUST, IMAGE_RECT_ADJUST));

    /** Render to image */
        QImage image( scene_rect.width() * IMAGE_EXPORT_SCALE_FACTOR,
                          scene_rect.height() * IMAGE_EXPORT_SCALE_FACTOR,
                                   QImage::Format_RGB32);
    image.fill( graph_view->palette().base().color().rgb());
    QPainter pp( &image);
        pp.setRenderHints( graph_view->renderHints());
    graph_view->scene()->render( &pp, image.rect(), scene_rect);

    delete graph_view;
    return image;
}


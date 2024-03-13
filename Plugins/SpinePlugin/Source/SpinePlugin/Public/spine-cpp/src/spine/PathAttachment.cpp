/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated July 28, 2023. Replaces all prior versions.
 *
 * Copyright (c) 2013-2023, Esoteric Software LLC
 *
 * Integration of the Spine Runtimes into software or otherwise creating
 * derivative works of the Spine Runtimes is permitted under the terms and
 * conditions of Section 2 of the Spine Editor License Agreement:
 * http://esotericsoftware.com/spine-editor-license
 *
 * Otherwise, it is permitted to integrate the Spine Runtimes into software or
 * otherwise create derivative works of the Spine Runtimes (collectively,
 * "Products"), provided that each user of the Products must obtain their own
 * Spine Editor license and redistribution of the Products in any form must
 * include this license and copyright notice.
 *
 * THE SPINE RUNTIMES ARE PROVIDED BY ESOTERIC SOFTWARE LLC "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ESOTERIC SOFTWARE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES,
 * BUSINESS INTERRUPTION, OR LOSS OF USE, DATA, OR PROFITS) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THE
 * SPINE RUNTIMES, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include <spine/PathAttachment.h>

using namespace spine;

RTTI_IMPL(PathAttachment, VertexAttachment)

PathAttachment::PathAttachment(const String &name) : VertexAttachment(name), _closed(false), _constantSpeed(false),
													 _color() {
}

Vector<float> &PathAttachment::getLengths() {
	return _lengths;
}

bool PathAttachment::isClosed() {
	return _closed;
}

void PathAttachment::setClosed(bool inValue) {
	_closed = inValue;
}

bool PathAttachment::isConstantSpeed() {
	return _constantSpeed;
}

void PathAttachment::setConstantSpeed(bool inValue) {
	_constantSpeed = inValue;
}

Color &PathAttachment::getColor() {
	return _color;
}

Attachment *PathAttachment::copy() {
	PathAttachment *copy = new (__FILE__, __LINE__) PathAttachment(getName());
	copyTo(copy);
	copy->_lengths.clearAndAddAll(_lengths);
	copy->_closed = _closed;
	copy->_constantSpeed = _constantSpeed;
	return copy;
}

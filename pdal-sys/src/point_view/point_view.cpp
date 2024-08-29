// MIT License
//
// Copyright (c) 2024 NUVIEW, Inc. <simeon.fitch@nuview.space>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "pdal-sys/src/point_view/point_view.hpp"

#include <memory>

namespace pdal_sys {
namespace point_view_set {
std::unique_ptr<PointViewSetIter> iter(const PointViewSet& set) {
    return std::make_unique<PointViewSetIter>(set);
}

PointViewSetIter::PointViewSetIter(const pdal::PointViewSet& views)
    : m_views(views), m_impl(m_views.cbegin()) {}

bool PointViewSetIter::hasNext() const { return (m_impl != m_views.cend()); }

pdal::PointViewPtr PointViewSetIter::next() {
    if (!hasNext()) {
        throw std::out_of_range("No more elements in iterator");
    } else {
        return *(m_impl++);
    }
}
}  // namespace point_view_set

namespace point_view {
std::unique_ptr<PointView> createPointView() {
    pdal::ColumnPointTable table;
    return std::make_unique<PointView>(table);
}
int id(const PointView& view) { return view.id(); }

rust::String proj4(const PointView& view) {
    auto sr = view.spatialReference();
    return sr.getProj4();
}

rust::String wkt(const PointView& view) {
    auto sr = view.spatialReference();
    return sr.getWKT();
}

const pdal::PointLayout& layout(const PointView& view) {
    // TODO: is this legit? Does it create a stable reference?
    return *view.layout();
}

pdal::PointLayout& layout_mut(PointView& view) {
    // TODO: is this legit? Does it create a stable reference?
    return *view.layout();
}

void register_dims(PointView& view, rust::Vec<pdal::Dimension::Id> dims) {
    for (auto dim : dims) {
        view.layout()->registerDim(dim);
    }
    fprintf(stderr, "Registered %d dimensions\n", dims.size());
    view.table().finalize();
}

size_t pointSizeForDims(const PointView& view,
                        const rust::Vec<core::DimTypeId>& dims) {
    size_t retval = 0;
    for (auto dim : dims) {
        retval += view.layout()->dimSize(dim);
    }
    return retval;
}

std::int8_t pointField_i8(const PointView& pv, core::DimTypeId dim,
                          pdal::PointId id) {
    return pv.getFieldAs<std::int8_t>(dim, id);
}
std::uint8_t pointField_u8(const PointView& pv, core::DimTypeId dim,
                           pdal::PointId id) {
    return pv.getFieldAs<std::uint8_t>(dim, id);
}
std::int16_t pointField_i16(const PointView& pv, core::DimTypeId dim,
                            pdal::PointId id) {
    return pv.getFieldAs<std::int16_t>(dim, id);
}
std::uint16_t pointField_u16(const PointView& pv, core::DimTypeId dim,
                             pdal::PointId id) {
    return pv.getFieldAs<std::uint16_t>(dim, id);
}
std::int32_t pointField_i32(const PointView& pv, core::DimTypeId dim,
                            pdal::PointId id) {
    return pv.getFieldAs<std::int32_t>(dim, id);
}
std::uint32_t pointField_u32(const PointView& pv, core::DimTypeId dim,
                             pdal::PointId id) {
    return pv.getFieldAs<std::uint32_t>(dim, id);
}
std::int64_t pointField_i64(const PointView& pv, core::DimTypeId dim,
                            pdal::PointId id) {
    return pv.getFieldAs<std::int64_t>(dim, id);
}
std::uint64_t pointField_u64(const PointView& pv, core::DimTypeId dim,
                             pdal::PointId id) {
    return pv.getFieldAs<std::uint64_t>(dim, id);
}
float pointField_f32(const PointView& pv, core::DimTypeId dim,
                     pdal::PointId id) {
    return pv.getFieldAs<float>(dim, id);
}
double pointField_f64(const PointView& pv, core::DimTypeId dim,
                      pdal::PointId id) {
    return pv.getFieldAs<double>(dim, id);
}

void setPointField_i8(PointView& pv, core::DimTypeId dim, pdal::PointId id,
                      std::int8_t val) {
    pv.setField(dim, id, val);
}
void setPointField_u8(PointView& pv, core::DimTypeId dim, pdal::PointId id,
                      std::uint8_t val) {
    pv.setField(dim, id, val);
}
void setPointField_i16(PointView& pv, core::DimTypeId dim, pdal::PointId id,
                       std::int16_t val) {
    pv.setField(dim, id, val);
}
void setPointField_u16(PointView& pv, core::DimTypeId dim, pdal::PointId id,
                       std::uint16_t val) {
    pv.setField(dim, id, val);
}
void setPointField_i32(PointView& pv, core::DimTypeId dim, pdal::PointId id,
                       std::int32_t val) {
    pv.setField(dim, id, val);
}
void setPointField_u32(PointView& pv, core::DimTypeId dim, pdal::PointId id,
                       std::uint32_t val) {
    pv.setField(dim, id, val);
}
void setPointField_i64(PointView& pv, core::DimTypeId dim, pdal::PointId id,
                       std::int64_t val) {
    pv.setField(dim, id, val);
}
void setPointField_u64(PointView& pv, core::DimTypeId dim, pdal::PointId id,
                       std::uint64_t val) {
    pv.setField(dim, id, val);
}
void setPointField_f32(PointView& pv, core::DimTypeId dim, pdal::PointId id,
                       float val) {
    pv.setField(dim, id, val);
}
void setPointField_f64(PointView& pv, core::DimTypeId dim, pdal::PointId id,
                       double val) {
    pv.setField(dim, id, val);
}
}  // namespace point_view

/*
namespace buffer_reader {
std::unique_ptr<BufferReader> make() {
    return std::make_unique<BufferReader>();
}
void addView(BufferReader& reader, pdal::PointViewPtr view) {
    reader.addView(view);
}
}  // namespace buffer_reader
*/
}  // namespace pdal_sys


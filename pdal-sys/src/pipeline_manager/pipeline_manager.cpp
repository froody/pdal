// MIT License
//
// Copyright (c) 2024 NUVIEW, Inc. <simeon.fitch@nuview.space>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
// OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "pdal-sys/src/pipeline_manager/pipeline_manager.hpp"

#include <sstream>

namespace pdal_sys {
    std::unique_ptr<PipelineManager> createPipelineManager() {
        return std::unique_ptr<PipelineManager>(new PipelineManager());
    }

    PipelineManager::PipelineManager() : m_impl(std::unique_ptr<pdal::PipelineManager>(new pdal::PipelineManager)) {}

    void PipelineManager::readPipeline(rust::Str json) {
        std::string str = std::string(json);
        std::istringstream is(str);
        m_impl->readPipeline(is);
    }
    void PipelineManager::readPipelineFromFile(rust::Str path) {
        m_impl->readPipeline(std::string(path));
    }

    bool PipelineManager::pipelineStreamable() const {
        return m_impl->pipelineStreamable();
    }

    std::size_t PipelineManager::execute() {
        return m_impl->execute();
    }

    void PipelineManager::executeStreamed() {
         m_impl->execute(pdal::ExecMode::PreferStream);
    }

    void PipelineManager::setupInputView(std::unique_ptr<pdal_sys::point_view::PointView> view) {
        std::shared_ptr<pdal::PointView> view_impl = std::move(view);
        auto stage = m_impl->getStage();
        pdal::BufferReader readerStage;
        readerStage.addView(view_impl);
        stage->setInput(readerStage);
    }

    using pdal_sys::point_view::PointView;
    std::unique_ptr<PointView> PipelineManager::getView() const {
        std::unique_ptr<PointView> view(new PointView(m_impl->pointTable()));
        return view;
    }

    using pdal_sys::point_view_set::PointViewSet;
    const PointViewSet& PipelineManager::views() const {
        return m_impl->views();
    }

    rust::String PipelineManager::metadata() const {
        std::stringstream strm;
        pdal::MetadataNode root = m_impl->getMetadata().clone("metadata");
        pdal::Utils::toJSON(root, strm);
        return rust::String(strm.str());
    }

    rust::String PipelineManager::schema() const {
        std::stringstream strm;
        pdal::MetadataNode root = m_impl->pointTable().layout()->toMetadata().clone("schema");
        pdal::Utils::toJSON(root, strm);
        return rust::String(strm.str());
    }

    rust::String PipelineManager::pipeline() const {
        std::stringstream strm;
        pdal::PipelineWriter::writePipeline(m_impl->getStage(), strm);
        return rust::String(strm.str());
    }
}

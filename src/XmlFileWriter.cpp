// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//



#include <stack>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <sstream>


#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "XmlFileWriter.hpp"

std::string format_xml_exception(int code) {
    std::ostringstream ss;
    ss << "XML error #" << code;
    return ss.str();
}

const xmlChar* toXmlChar (const std::string& str) {
    return BAD_CAST str.c_str();
}


void XmlErrorToException(void* /*ctx*/, xmlErrorPtr error) {
    std::ostringstream ss;
    ss << "XML error: '" << error->message << "' "
      << "in " << error->file 
      << ":" << error->line;
    throw std::runtime_error(ss.str());
}


XmlFileWriter::XmlFileWriter() :w(nullptr), stack() {
    LIBXML_TEST_VERSION;
    xmlSetStructuredErrorFunc(nullptr, XmlErrorToException);
}

XmlFileWriter::~XmlFileWriter() {
    // XXX shouldn't happen
    if (isOpen()) {
        close();
    }
}

void XmlFileWriter::open(const std::string& filename) {
    std::cerr << "XmlFileWriter: writing to " << filename << std::endl;
    w = xmlNewTextWriterFilename((filename).c_str(), 0);
    if (w == nullptr) throw std::runtime_error("Failed to create XML writer");
    xmlTextWriterSetIndent(w, 1);
}

bool XmlFileWriter::isOpen() {
    return w != nullptr;
}

void XmlFileWriter::close() {
    if (w == nullptr) {
        throw std::runtime_error("Protocol mismatch: closing already closed XML document");
    }
    xmlFreeTextWriter(w);
    w = nullptr;
}

void XmlFileWriter::startDocument(const std::string& encoding) {
    int rc = xmlTextWriterStartDocument(w, NULL, encoding.c_str(), NULL);
    if (rc < 0) throw std::runtime_error(format_xml_exception(rc));
}
void XmlFileWriter::endDocument() {
    int rc = xmlTextWriterEndDocument(w);
    if (rc < 0) throw std::runtime_error(format_xml_exception(rc));
}

void XmlFileWriter::startElement(const std::string& name) {
    int rc = xmlTextWriterStartElement(w, toXmlChar(name));
    if (rc < 0) throw std::runtime_error(format_xml_exception(rc));
    stack.push(name);
}

void XmlFileWriter::endElement(const std::string& name) {
    int rc = xmlTextWriterEndElement(w);
    if (rc < 0) throw std::runtime_error(format_xml_exception(rc));
    if (name.size()) {
        if (name != stack.top()) throw std::runtime_error("XML Tag mismatch (expected a <" + stack.top() + ">, got a <" + name +">)");
    }
    stack.pop();
}

void XmlFileWriter::writeAttribute(const std::string& name, const std::string& value) {
    int rc = xmlTextWriterWriteAttribute(w, toXmlChar(name), toXmlChar(value));
    if (rc < 0) throw std::runtime_error(format_xml_exception(rc));
}

void XmlFileWriter::writeElement(const std::string& name, const std::string& value) {
    int rc = xmlTextWriterWriteElement(w, toXmlChar(name), toXmlChar(value));
    if (rc < 0) throw std::runtime_error(format_xml_exception(rc));
}

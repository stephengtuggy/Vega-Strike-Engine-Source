/*
 * cargo.cpp
 *
 * Vega Strike - Space Simulation, Combat and Trading
 * Copyright (C) 2001-2025 The Vega Strike Contributors:
 * Project creator: Daniel Horn
 * Original development team: As listed in the AUTHORS file
 * Current development team: Roy Falk, Benjamen R. Meyer, Stephen G. Tuggy
 *
 *
 * https://github.com/vegastrike/Vega-Strike-Engine-Source
 *
 * This file is part of Vega Strike.
 *
 * Vega Strike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vega Strike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vega Strike.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "resource/cargo.h"


Cargo::Cargo(): Product() {
    mass = 0;
    volume = 0;
    mission = false;
    installed = false;
    functionality = max_functionality = 1.0f;
}

Cargo::Cargo(std::string name, std::string category, float price, int quantity,
             float mass, float volume, float functionality, float max_functionality,
             bool mission, bool installed, bool integral) :
        Product(name, quantity, price), category(category) {
    this->mass = mass;
    this->volume = volume;
    this->mission = false;
    this->installed = false;
    this->functionality = functionality;
    this->max_functionality = max_functionality;
    this->mission = mission;
    this->installed = installed;
    this->integral = integral;
}

float Cargo::GetFunctionality() {
    return functionality;
}

float Cargo::GetMaxFunctionality() {
    return max_functionality;
}

void Cargo::SetDescription(const std::string &description) {
    this->description = description;
}

void Cargo::SetFunctionality(float func) {
    functionality = func;
}

void Cargo::SetInstalled(bool installed) {
    this->installed = installed;
}

void Cargo::SetIntegral(bool installed) {
    this->integral = installed;
}

void Cargo::SetMaxFunctionality(float func) {
    max_functionality = func;
}

void Cargo::SetMissionFlag(bool flag) {
    this->mission = flag;
}

void Cargo::SetPrice(float price) {
    this->price = price;
}

void Cargo::SetMass(float mass) {
    this->mass = mass;
}

void Cargo::SetVolume(float vol) {
    this->volume = vol;
}

void Cargo::SetQuantity(int quantity) {
    this->quantity = quantity;
}

void Cargo::SetContent(const std::string &content) {
    this->name = content;
}

void Cargo::SetCategory(const std::string &category) {
    this->category = category;
}

bool Cargo::GetMissionFlag() const {
    return this->mission;
}

bool Cargo::GetInstalled() const {
    return this->installed;
}

bool Cargo::GetIntegral() const {
    return this->integral;
}

const std::string &Cargo::GetCategory() const {
    return category;
}

const std::string &Cargo::GetContent() const {
    return name;
}

const std::string &Cargo::GetDescription() const {
    return description;
}



std::string Cargo::GetCategoryPython() {
    return category;
}

std::string Cargo::GetContentPython() {
    return name;
}

std::string Cargo::GetDescriptionPython() {
    return description;
}

int Cargo::GetQuantity() const {
    return (quantity.Value());
}

float Cargo::GetVolume() const {
    return volume;
}

float Cargo::GetMass() const {
    return mass;
}

float Cargo::GetPrice() const {
    return price;
}




bool Cargo::operator==(const Cargo &other) const {
    return name == other.name;
}

bool Cargo::operator<(const Cargo &other) const {
    return (category == other.category) ? (name < other.name) : (category < other.category);
}


void Cargo::Add(int quantity) {
    this->quantity += quantity;
}
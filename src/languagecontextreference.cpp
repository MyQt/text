/*
 * Copyright © 2017 Andrew Penkrat
 *
 * This file is part of Liri Text.
 *
 * Liri Text is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Liri Text is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Liri Text.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "languagecontextreference.h"
#include "languagecontextcontainer.h"
#include "languagecontextsimple.h"

LanguageContextReference::LanguageContextReference() { }

LanguageContextReference::~LanguageContextReference() {
    if(context)
        resolveCircularDeps({this});
}

void LanguageContextReference::resolveCircularDeps(QList<LanguageContextReference *> stack) {
    auto current = stack.last();
    if(current->context->type == LanguageContext::Simple) {
        auto simple = current->context.staticCast<LanguageContextSimple>();
        resolveCircularDeps(stack, simple);
    }
    if(current->context->type == LanguageContext::Container) {
        auto container = current->context.staticCast<LanguageContextContainer>();
        resolveCircularDeps(stack, container);
    }
}

template<class ContextType>
void LanguageContextReference::resolveCircularDeps(QList<LanguageContextReference *> stack, QSharedPointer<ContextType> current) {
    for (auto inc : current->includes) {
        if(stack.contains(inc.data())) {
            current->includes.removeOne(inc);
            continue;
        }
        auto newStack = stack;
        newStack.append(inc.data());
        resolveCircularDeps(newStack);
    }
}
#ifndef ZAUDIO_CONFIG_HPP
#define ZAUDIO_CONFIG_HPP

/*
 * This file is part of zaudio.
 *
 *  zaudio is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  zaudio is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with zaudio.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef _WIN32
# define ZAUDIO_EXPORT __declspec(dllexport)
#elif _WIN64
# define ZAUDIO_EXPORT __declspec(dllexport)
#else
# define ZAUDIO_EXPORT
#endif

#endif // ifndef ZAUDIO_CONFIG_HPP

//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package src.behaviorDevice;

simple Switching
{
    parameters:
        @display("i=block/switch");
        @signal[sourceAddress](type="long");
        @signal[drop](type="long");
        @signal[outputIf](type="long");
        @statistics[sourceAddress](title="source address of arrived packets"; interpolationmode=none; record=vector?);
        @statistic[drop](title="dropped packet byte length"; unit=bytes; record=vector?,count,sum; interpolationmode=none);
        @statistic[outputIf](title="output interface for each routed packet"; record=histogram; interpolationmode=none);
    gates:
        input localIn;
        output localOut;
        input in[];
        output out[];
}

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

package coolNetworks;

//
// TODO auto-generated module
//
simple Node
{
    parameters:
        int numberofNodes;
        volatile int MAX_SEQ = default(7);
        volatile double TIMEOUT @unit(s) = default(2.0s);
        volatile double NEW_MESSAGE_TIME @unit(s) = default(1.0s);
        
    gates:
        output out[];
        input in[];
        //inout parentConnection;

}








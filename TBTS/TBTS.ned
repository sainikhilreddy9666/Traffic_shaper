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


network TBTS
{
    @display("bgb=271,253;i=abstract/switch");

    submodules:
        tic_Trans: Tic_Trans {
            @display("p=32,127;i=block/source");
        }
        toc_Rec: Toc_Rec {
            @display("p=228,128");
        }
        traffic_Shaper: Traffic_Shaper {
            @display("p=134,128;i=block/boundedqueue;is=l");
        }
        token_Generator: Token_Generator {

            @display("p=134,39;i=block/bucket");
        }

    connections:
        tic_Trans.out --> traffic_Shaper.in;
        token_Generator.out --> traffic_Shaper.Token_in;
        traffic_Shaper.Token_out --> token_Generator.in;
        traffic_Shaper.out --> toc_Rec.in;
}


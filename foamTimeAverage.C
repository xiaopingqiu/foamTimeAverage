/*---------------------------------------------------------------------------*\
 *=========                 |
 *\\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
 *\\     /   O peration     |
 *\\    /    A nd           | Copyright (C) 1991-2009 OpenCFD Ltd.
 *\\   /     M anipulation  |
 * -------------------------------------------------------------------------------
 license
 This file is part of OpenFOAM.

 OpenFOAM is free software; you can redistribute it and/or modify it
 under the terms of the GNU General Public License as published by the
 Free Software Foundation; either version 2 of the License, or (at your
 option) any later version.

 OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License
 along with OpenFOAM; if not, write to the Free Software Foundation,
 Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Application
   foamTimeAverage

Author
  Xiaoping Qiu 
  q.giskard@gmail.com

Description
Calculates the time average  of the specified volField over the specified time range.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class FieldType>
void calcTimeAverage
(
    fvMesh& mesh,
    const IOobject& fieldHeader,
    const word& fieldName,
    Time& runTime,
    instantList& timeDirs,
    bool& done
)
{
    label nfield = 0;
    const word meanFieldName = fieldName + "_mean"; 

    //Info << "class name = " << fieldHeader.headerClassName() << endl;
    //Info << "typeName = " << FieldType::typeName << endl;
    if(!done && fieldHeader.headerClassName() == FieldType::typeName)
    {
	FieldType dummy
	    (
		IOobject
		(
		    fieldName,
		    runTime.timeName(),
		    mesh,
		    IOobject::MUST_READ
		),
		mesh
	    );

	FieldType meanField
	    (
		IOobject
		(
		    meanFieldName,
		    runTime.timeName(),
		    mesh,
		    IOobject::NO_READ
		),
		dummy
	    );

	meanField *= scalar(0.0);

	forAll(timeDirs, timeI)
	{
	    runTime.setTime(timeDirs[timeI], timeI);
	    Info << "Time = " << runTime.timeName() <<endl;

	    IOobject io
		(
		    fieldName,
		    runTime.timeName(),
		    mesh,
		    IOobject::MUST_READ
		);

	    if (io.headerOk())
	    {
		mesh.readUpdate();

		if(!done && io.headerClassName() == FieldType::typeName)
		{
		    Info << "   Reading " << io.headerClassName() << " " <<io.name() << endl;

		    FieldType field(io, mesh);

		    meanField += field;
		    nfield++;
		}
	    }
	    else
	    {
		Info << "   No Field " << fieldName << endl; 
	    }
	}

	if(nfield > 0)
	{
	    Info << "number of field = " << nfield << endl;
	    meanField /= nfield;
	}

	Info<< "writing to timeDir " << runTime.timeName()  << endl;
	meanField.write();
	done = true;

    }
}
// Main program:

int main(int argc, char *argv[])
{

    Foam::timeSelector::addOptions();
    #include "addRegionOption.H"
    Foam::argList::validArgs.append("fieldName");

#   include "setRootCase.H"
#   include "createTime.H"
    instantList timeDirs = timeSelector::select0(runTime, args);
    runTime.setTime(timeDirs[0], 0);
#   include "createNamedMesh.H"

    // get filename from command line
    const word fieldName = args[1];
    bool done = false;

    IOobject fieldHeader
	(
	    fieldName,
	    runTime.timeName(),
	    mesh,
	    IOobject::MUST_READ
	);
    if(fieldHeader.headerOk())//very important!
    {
	calcTimeAverage<volScalarField>(mesh, fieldHeader, fieldName, runTime, timeDirs, done);
	calcTimeAverage<volVectorField>(mesh, fieldHeader, fieldName, runTime, timeDirs, done);
	calcTimeAverage<volTensorField>(mesh, fieldHeader, fieldName, runTime, timeDirs, done);
	calcTimeAverage<volSymmTensorField>(mesh, fieldHeader, fieldName, runTime, timeDirs, done);
	calcTimeAverage<volSphericalTensorField>(mesh, fieldHeader, fieldName, runTime, timeDirs, done);
    }
    else
    {
	Info<< " Error! No field " << fieldName << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}

// ************************************************************************* //

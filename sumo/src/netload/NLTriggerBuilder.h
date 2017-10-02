/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2002-2017 German Aerospace Center (DLR) and others.
/****************************************************************************/
//
//   This program and the accompanying materials
//   are made available under the terms of the Eclipse Public License v2.0
//   which accompanies this distribution, and is available at
//   http://www.eclipse.org/legal/epl-v20.html
//
/****************************************************************************/
/// @file    NLTriggerBuilder.h
/// @author  Daniel Krajzewicz
/// @author  Tino Morenz
/// @author  Eric Nicolay
/// @author  Michael Behrisch
/// @author  Jakob Erdmann
/// @date    Thu, 17 Oct 2002
/// @version $Id$
///
// Builds trigger objects for microsim
/****************************************************************************/
#ifndef NLTriggerBuilder_h
#define NLTriggerBuilder_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <string>
#include <vector>


// ===========================================================================
// class declarations
// ===========================================================================
class MSTrigger;
class MSNet;
class MSLaneSpeedTrigger;
class NLHandler;
class MSTriggeredRerouter;
class MSLane;
class MSEdge;
class MSStoppingPlace;
class MSCalibrator;
class MSRouteProbe;

class METriggeredCalibrator;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class NLTriggerBuilder
 * @brief Builds trigger objects for microsim
 *
 * The building methods may be overridden, to build guisim-instances of the triggers,
 *  for example.
 *
 */
class NLTriggerBuilder {
public:
    /// @brief Constructor
    NLTriggerBuilder();


    /// @brief Destructor
    virtual ~NLTriggerBuilder();


    /** @brief Sets the parent handler to use for nested parsing
     *
     * @param[in] handler The netload handler to set
     */
    void setHandler(NLHandler* handler);


    /** @brief Builds a vaporization
     *
     * Parses the attributes, reporting errors if the time values are false
     *  or the edge is not known.
     * Instatiates events for enabling and disabling the vaporization otherwise.
     *
     * @param[in] attrs SAX-attributes which define the vaporizer
     * @note recheck throwing the exception
     */
    void buildVaporizer(const SUMOSAXAttributes& attrs);


    /// @name parsing methods
    ///
    /// These methods parse the attributes for each of the described trigger
    ///  and call the according methods to build the trigger
    //@{
    /** @brief Parses his values and builds a lane speed trigger
     *
     * If one of the declaration values is errornous, an InvalidArgument is thrown.
     *
     * If the XML-file parsed during initialisation is errornous, and the
     *  MSLaneSpeedTrigger-constructor throws a ProcessError due to this, this
     *  exception is catched and an InvalidArgument with the message given in the
     *  ProcessError is thrown.
     *
     * @param[in] net The network the lane speed trigger belongs to
     * @param[in] attrs SAX-attributes which define the trigger
     * @param[in] base The base path
     * @exception InvalidArgument If a parameter (lane/position) is not valid or the read definition is errornous
     * @see buildLaneSpeedTrigger
     */
    void parseAndBuildLaneSpeedTrigger(MSNet& net, const SUMOSAXAttributes& attrs,
                                       const std::string& base);


    /** @brief Parses his values and builds a rerouter
     *
     * @param[in] net The network the rerouter belongs to
     * @param[in] attrs SAX-attributes which define the trigger
     * @param[in] base The base path
     * @exception InvalidArgument If a parameter (edge) is not valid
     */
    void parseAndBuildRerouter(MSNet& net, const SUMOSAXAttributes& attrs,
                               const std::string& base);


    /** @brief Parses the values and builds a stopping places for busses, trains or container vehicles
     *
     * @param[in] net The network the stop belongs to
     * @param[in] attrs SAX-attributes which define the stop
     * @param[in] element which kind of stop is to be built
     * @exception InvalidArgument If a parameter (lane/position) is not valid
     */
    void parseAndBuildStoppingPlace(MSNet& net, const SUMOSAXAttributes& attrs, const SumoXMLTag element);


    /** @brief Begin a parking area
     *
     * Simply calls the MSParkingArea constructor.
     *
     * @param[in] net The net the parking area belongs to
     * @param[in] id The id of the parking area
     * @param[in] lines Names of the lines that halt on this parking area
     * @param[in] lane The lane the parking area is placed on
     * @param[in] frompos Begin position of the parking area on the lane
     * @param[in] topos End position of the parking area on the lane
     * @param[in] capacity Capacity of the parking area
     * @param[in] width Width of the default lot rectangle
     * @param[in] length Length of the default lot rectangle
     * @param[in] angle Angle of the default lot rectangle
     * @exception InvalidArgument If the parking area can not be added to the net (is duplicate)
     */
    virtual void beginParkingArea(MSNet& net,
                                  const std::string& id, const std::vector<std::string>& lines,
                                  MSLane* lane, double frompos, double topos,
                                  unsigned int capacity,
                                  double width, double length, double angle);


    /** @brief Add a lot entry to current parking area
     *
     * Simply calls the addLotEntry method for current parking area.
     *
     * @param[in] x X position of the lot center
     * @param[in] y Y position of the lot center
     * @param[in] z Z position of the lot center
     * @param[in] width Width of the lot rectangle
     * @param[in] length Length of the lot rectangle
     * @param[in] angle Angle of the lot rectangle
     * @exception InvalidArgument If the current parking area is 0
     */
    void addLotEntry(double x, double y, double z,
                     double width, double length, double angle);



    /** @brief Parses the values and adds an access point to the currently parsed stopping place
     *
     * @param[in] net The network the stop belongs to
     * @param[in] attrs SAX-attributes which define the access
     * @exception InvalidArgument If a parameter (lane/position) is not valid
     */
    void addAccess(MSNet& net, const SUMOSAXAttributes& attrs);


    /** @brief Parses his values and builds a parking area
     *
     * @param[in] net The network the parking area belongs to
     * @param[in] attrs SAX-attributes which define the trigger
     * @exception InvalidArgument If a parameter (lane/position) is not valid
     */
    void parseAndBeginParkingArea(MSNet& net, const SUMOSAXAttributes& attrs);


    /** @brief Parses his values and adds a lot entry to current parking area
     *
     * @param[in] attrs SAX-attributes which define the lot entry
     */
    void parseAndAddLotEntry(const SUMOSAXAttributes& attrs);


    /** @brief End a parking area
     *
     * @exception InvalidArgument If the current parking area is 0
     */
    virtual void endParkingArea();


    /** @brief Parses his values and builds a charging station
     *
     * @param[in] net The network the charging station belongs to
     * @param[in] attrs SAXattributes which define the trigger
     * @exception InvalidArgument If a parameter (lane/position) is not valid
     */
    void parseAndBuildChargingStation(MSNet& net, const SUMOSAXAttributes& attrs);

    /** @brief Parses his values and builds a mesoscopic or microscopic calibrator
     *
     * @param[in] net The network the calibrator belongs to
     * @param[in] attrs SAX-attributes which define the trigger
     * @param[in] base The base path
     * @exception InvalidArgument If a parameter (edge/position) is not valid
     */
    void parseAndBuildCalibrator(MSNet& net, const SUMOSAXAttributes& attrs,
                                 const std::string& base);
    //@}

protected:
    /// @name building methods
    ///
    /// Called with parsed values, these methods build the trigger.
    ///
    /// These methods should be overriden for the gui loader in order
    ///  to build visualizable versions of the triggers.
    ///
    /// In most cases, these methods only call the constructor.
    //@{

    /** @brief Builds a lane speed trigger
     *
     * Simply calls the MSLaneSpeedTrigger constructor.
     *
     * @param[in] net The net the lane speed trigger belongs to
     * @param[in] id The id of the lane speed trigger
     * @param[in] destLanes List of lanes affected by this speed trigger
     * @param[in] file Name of the file to read the speeds to set from
     * @see MSLaneSpeedTrigger
     * @exception ProcessError If the XML definition file is errornous
     */
    virtual MSLaneSpeedTrigger* buildLaneSpeedTrigger(MSNet& net,
            const std::string& id, const std::vector<MSLane*>& destLanes,
            const std::string& file);


    /** @brief Builds a stopping place
     *
     * Simply calls the MSStoppingPlace constructor.
     *
     * @param[in] net The net the stop belongs to
     * @param[in] id The id of the stop
     * @param[in] lines Names of the lines that halt on this bus stop
     * @param[in] lane The lane the stop is placed on
     * @param[in] frompos Begin position of the stop on the lane
     * @param[in] topos End position of the stop on the lane
     * @param[in] element which kind of stop is to be built
     * @exception InvalidArgument If the stop can not be added to the net (is duplicate)
     */
    virtual void buildStoppingPlace(MSNet& net, std::string id, std::vector<std::string> lines, MSLane* lane,
                                    double frompos, double topos, const SumoXMLTag element, std::string string);

    /** @brief Builds a charging Station
     *
     * Simply calls the MSBusStop constructor.
     *
     * @param[in] net The net the charging Station belongs to
     * @param[in] id The id of the charging Station
     * @param[in] lane The lane the charging Station is placed on
     * @param[in] frompos Begin position of the charging Station on the lane
     * @param[in] topos End position of the charging Station on the lane
     * @param[in] chargingPower
     * @param[in] efficiency
     * @param[in] chargeInTransit
     * @param[in] ChargeDelay
     * @exception InvalidArgument If the charging Station can not be added to the net (is duplicate)
     */
    virtual void buildChargingStation(MSNet& net, const std::string& id, MSLane* lane, double frompos, double topos,
                                      double chargingPower, double efficiency, bool chargeInTransit, int ChargeDelay);

    /** @brief builds a microscopic calibrator
     *
     * Simply calls the MSCalibrator constructor.
     *
     * @param[in] net The net the calibrator belongs to
     * @param[in] id The id of the calibrator
     * @param[in] edge The edge the calibrator is placed at
     * @param[in] pos The position on the edge the calibrator lies at
     * @param[in] file The file to read the flows from
     * @todo Is the position correct/needed
     */
    virtual MSCalibrator* buildCalibrator(MSNet& net,
                                          const std::string& id, MSEdge* edge, MSLane* lane, double pos,
                                          const std::string& file, const std::string& outfile,
                                          const SUMOTime freq, const MSRouteProbe* probe);

    /** @brief builds a mesoscopic calibrator
     *
     * Simply calls the METriggeredCalibrator constructor.
     *
     * @param[in] net The net the calibrator belongs to
     * @param[in] id The id of the calibrator
     * @param[in] edge The edge the calibrator is placed at
     * @param[in] pos The position on the edge the calibrator lies at
     * @param[in] file The file to read the flows from
     * @todo Is the position correct/needed
     */
    virtual METriggeredCalibrator* buildMECalibrator(MSNet& net,
            const std::string& id, const MSEdge* edge, double pos,
            const std::string& file, const std::string& outfile,
            const SUMOTime freq, MSRouteProbe* probe);


    /** @brief builds an rerouter
     *
     * Simply calls the MSTriggeredRerouter constructor.
     *
     * @param[in] net The net the rerouter belongs to
     * @param[in] id The id of the rerouter
     * @param[in] edges The edges the rerouter is placed at
     * @param[in] prob The probability the rerouter reoutes vehicles with
     * @param[in] file The file to read the reroute definitions from
     */
    virtual MSTriggeredRerouter* buildRerouter(MSNet& net,
            const std::string& id, MSEdgeVector& edges,
            double prob, const std::string& file, bool off);
    //@}


protected:
    /// @name helper method for obtaining and checking values
    /// @{

    /** @brief Helper method to obtain the filename
     *
     * Retrieves "file" from attributes, checks whether it is absolute
     *  and extends it by the given base path if not. Returns this
     *  information.
     *
     * @param[in] attrs The attributes to obtain the file name from
     * @param[in] base The base path (the path the loaded additional file lies in)
     * @return The (expanded) path to the named file
     * @todo Recheck usage of the helper class
     */
    std::string getFileName(const SUMOSAXAttributes& attrs,
                            const std::string& base,
                            const bool allowEmpty = false);


    /** @brief Returns the lane defined by attribute "lane"
     *
     * Retrieves the lane id from the given attrs. Tries to retrieve the lane,
     *  throws an InvalidArgument if it does not exist.
     *
     * @param[in] attrs The attributes to obtain the lane id from
     * @param[in] tt The trigger type (for user output)
     * @param[in] tid The trigger id (for user output)
     * @return The named lane if it is known
     * @exception InvalidArgument If the named lane does not exist or a lane is not named
     */
    MSLane* getLane(const SUMOSAXAttributes& attrs,
                    const std::string& tt, const std::string& tid);


    /** @brief returns the position on the lane checking it
     *
     * This method extracts the position, checks whether it shall be mirrored
     *  and checks whether it is within the lane. If not, an error is reported
     *  and a InvalidArgument is thrown.
     *
     * @param[in] attrs The attributes to obtain the position from
     * @param[in] lane The lane the position shall be valid for
     * @param[in] tt The trigger type (for user output)
     * @param[in] tid The trigger id (for user output)
     * @return The position on the lane
     * @exception InvalidArgument If the position is beyond the lane
     */
    double getPosition(const SUMOSAXAttributes& attrs,
                       MSLane* lane, const std::string& tt, const std::string& tid, MSEdge* edge = 0);
    /// @}


protected:
    /// @brief The parent handler to set for subhandlers
    NLHandler* myHandler;
    /// @brief definition of the currently parsed parking area
    MSParkingArea* myParkingArea;
    /// @brief The currently parsed stop to add access points to
    MSStoppingPlace* myCurrentStop;
};


#endif

/****************************************************************************/

